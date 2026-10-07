param(
    [string]$EngineRoot = 'C:/Program Files/Epic Games/UE_5.7',
    [switch]$RegenerateAssets
)

$ErrorActionPreference = 'Stop'
$taskRoot = Split-Path -Parent $PSScriptRoot
$taskProject = Join-Path $taskRoot 'simple_proj.uproject'
$taskLogDirectory = Join-Path $taskRoot 'Saved/Logs'
$taskArchive = Join-Path $taskRoot 'Saved/Packaged/OWTPlayground'
$taskEditor = Join-Path $EngineRoot 'Engine/Binaries/Win64/UnrealEditor-Cmd.exe'
$taskBuild = Join-Path $EngineRoot 'Engine/Build/BatchFiles/Build.bat'
$taskUat = Join-Path $EngineRoot 'Engine/Build/BatchFiles/RunUAT.bat'

foreach ($taskRequired in @($taskProject, $taskEditor, $taskBuild, $taskUat)) {
    if (-not (Test-Path -LiteralPath $taskRequired)) {
        throw "Required file not found: $taskRequired"
    }
}

New-Item -ItemType Directory -Path $taskLogDirectory -Force | Out-Null

& $taskBuild simple_projEditor Win64 Development "-Project=$taskProject" -WaitMutex -NoHotReloadFromIDE 2>&1 |
    Tee-Object -FilePath (Join-Path $taskLogDirectory 'Playground_EditorBuild.log')
if ($LASTEXITCODE -ne 0) {
    throw "Editor build failed: $LASTEXITCODE"
}

$taskGenerateArguments = @(
    $taskProject, '-run=CreateOWTPlayground', '-unattended', '-nop4', '-nosound',
    '-nullrhi', "-abslog=$(Join-Path $taskLogDirectory 'Playground_CreateAssets.log')"
)
if ($RegenerateAssets) {
    $taskGenerateArguments += '-Regenerate'
}
elseif (Test-Path -LiteralPath (Join-Path $taskRoot 'Content/OWTPlayground/Maps/L_OWTPlayground.umap')) {
    $taskGenerateArguments += '-ValidateOnly'
}

& $taskEditor @taskGenerateArguments
if ($LASTEXITCODE -ne 0) {
    throw "Playground asset generation/validation failed: $LASTEXITCODE"
}

$taskPackageArguments = @(
    'BuildCookRun', "-project=$taskProject", '-noP4', '-platform=Win64',
    '-clientconfig=Development', '-build', '-cook', '-stage', '-pak', '-archive',
    "-archivedirectory=$taskArchive", "-stagingdirectory=$(Join-Path $taskRoot 'Saved/PlaygroundStage')",
    '-map=/Game/OWTPlayground/Maps/L_OWTPlayground+/Game/OWTPlayground/Maps/L_OWTStress+/Game/VTBOWT/Maps/L_OWTEditSample',
    '-unattended', '-utf8output'
)
& $taskUat @taskPackageArguments 2>&1 |
    Tee-Object -FilePath (Join-Path $taskLogDirectory 'Playground_Package.log')
if ($LASTEXITCODE -ne 0) {
    throw "Playground package failed: $LASTEXITCODE"
}

Write-Output "Package ready: $taskArchive/Windows/simple_proj.exe"
