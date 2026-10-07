param(
    [ValidateSet('Smoke', 'Stress', 'Navigation', 'All')]
    [string]$Suite = 'All',
    [int]$TimeoutSeconds = 1200
)

$ErrorActionPreference = 'Stop'
$taskRoot = Split-Path -Parent $PSScriptRoot
$taskExecutable = Join-Path $taskRoot 'Saved/Packaged/OWTPlayground/Windows/simple_proj.exe'
if (-not (Test-Path -LiteralPath $taskExecutable)) {
    throw 'Build the Playground package before running its tests.'
}
if ($TimeoutSeconds -lt 60) {
    throw 'TimeoutSeconds must be at least 60.'
}

$taskSuites = @('Smoke', 'Stress', 'Navigation')
if ($Suite -ne 'All') {
    $taskSuites = @($Suite)
}

foreach ($taskSuite in $taskSuites) {
    $taskMap = '/Game/OWTPlayground/Maps/L_OWTPlayground'
    if ($taskSuite -eq 'Stress') {
        $taskMap = '/Game/OWTPlayground/Maps/L_OWTStress'
    }
    $taskRunId = Get-Date -Format 'yyyyMMdd_HHmmss'
    $taskReportDirectory = Join-Path $taskRoot "Saved/Automation/Playground_Packaged_${taskSuite}_$taskRunId"
    $taskLog = Join-Path $taskRoot "Saved/Logs/Playground_Packaged_${taskSuite}_$taskRunId.log"
    $taskArguments = @(
        $taskMap, '-unattended', '-nop4', '-nosound', '-RenderOffscreen',
        '-windowed', '-forceres', '-ResX=1600', '-ResY=900',
        ('-ExecCmds="Automation RunTests OWT.Playground.' + $taskSuite + '"'),
        '-TestExit="Automation Test Queue Empty"',
        ('-ReportExportPath="' + $taskReportDirectory + '"'),
        ('-abslog="' + $taskLog + '"')
    )
    Write-Output "Running $taskSuite with the real graphics renderer: $taskMap"
    $taskProcess = Start-Process -FilePath $taskExecutable -ArgumentList $taskArguments -WindowStyle Hidden -PassThru
    if (-not $taskProcess.WaitForExit($TimeoutSeconds * 1000)) {
        Stop-Process -Id $taskProcess.Id -Force
        throw "Only this test process was stopped after timeout. Log: $taskLog"
    }
    $taskProcess.Refresh()
    if ($taskProcess.ExitCode -ne 0) {
        throw "$taskSuite process exited with $($taskProcess.ExitCode). Log: $taskLog"
    }
    $taskIndex = Join-Path $taskReportDirectory 'index.json'
    if (-not (Test-Path -LiteralPath $taskIndex)) {
        throw "Automation report missing: $taskIndex"
    }
    $taskReport = Get-Content -LiteralPath $taskIndex -Raw | ConvertFrom-Json
    if ($taskReport.failed -gt 0) {
        throw "$taskSuite has failed tests. Report: $taskIndex"
    }
    if ($taskReport.notRun -gt 0) {
        throw "$taskSuite has tests that did not run. Report: $taskIndex"
    }
    if (($taskReport.succeeded + $taskReport.succeededWithWarnings) -lt 1) {
        throw "$taskSuite ran no successful tests. Report: $taskIndex"
    }
    Write-Output "$taskSuite completed. Report: $taskIndex"
}
