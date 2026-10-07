param(
    [ValidateSet('Showroom', 'Stress')]
    [string]$Level = 'Showroom'
)

$ErrorActionPreference = 'Stop'
$taskRoot = Split-Path -Parent $PSScriptRoot
$taskExecutable = Join-Path $taskRoot 'Saved/Packaged/OWTPlayground/Windows/simple_proj.exe'
if (-not (Test-Path -LiteralPath $taskExecutable)) {
    throw "Packaged executable not found. Run Scripts/Build-Playground.ps1 first."
}

$taskMap = '/Game/OWTPlayground/Maps/L_OWTPlayground'
if ($Level -eq 'Stress') {
    $taskMap = '/Game/OWTPlayground/Maps/L_OWTStress'
}

# This is the interactive game requested by the user, not a background helper.
Start-Process -FilePath $taskExecutable -ArgumentList @($taskMap, '-windowed', '-ResX=1600', '-ResY=900')
