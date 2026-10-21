param(
    [Parameter(Mandatory)] [string]$Tag,
    [string]$EngineRoot = 'C:/Program Files/Epic Games/UE_5.7'
)

$taskRoot = Split-Path -Parent $PSScriptRoot
$taskProject = Join-Path $taskRoot 'simple_proj.uproject'
$taskEditor = Join-Path $EngineRoot 'Engine/Binaries/Win64/UnrealEditor-Cmd.exe'
$taskLogDirectory = Join-Path $taskRoot 'Saved/Logs/Duplication'
New-Item -ItemType Directory -Path $taskLogDirectory -Force | Out-Null

# PCGSchedulerHierarchy hits an engine assert that terminates the process, so it runs on its own.
$taskRuns = @(
    @{ Name = 'runtime'; Tests = 'OWT.EventCore+OWT.Runtime.A+OWT.Runtime.D+OWT.Runtime.M+OWT.Runtime.N+OWT.Runtime.O+OWT.Runtime.PCGC+OWT.Runtime.PCGP+OWT.Runtime.PCGR+OWT.Runtime.V' },
    @{ Name = 'scheduler'; Tests = 'OWT.Runtime.PCGSchedulerHierarchy' }
)
foreach ($taskRun in $taskRuns) {
    $taskLog = Join-Path $taskLogDirectory "test_$($Tag)_$($taskRun.Name).log"
    & $taskEditor $taskProject "-ExecCmds=Automation RunTests $($taskRun.Tests); Quit" -TestExit='Automation Test Queue Empty' `
        -unattended -nullrhi -nosound -nop4 -nosplash "-abslog=$taskLog" *> $null
    "== $($taskRun.Name) exit=$LASTEXITCODE"
    Select-String -Path $taskLog -Pattern 'Test Completed\. Result=\{(\w+)\} Name=\{([^}]*)\}' |
        ForEach-Object { '{0,-8} {1}' -f $_.Matches[0].Groups[1].Value, $_.Matches[0].Groups[2].Value }
    if (Select-String -Path $taskLog -Pattern 'Critical error' -Quiet) {
        'CRASH: ' + ((Select-String -Path $taskLog -Pattern 'Assertion failed|Fatal error' | Select-Object -First 1).Line)
    }
}

$taskCommandletLog = Join-Path $taskLogDirectory "commandlet_$Tag.log"
& $taskEditor $taskProject -run=ValidateOWTAttributes -unattended -nullrhi -nosound -nop4 "-abslog=$taskCommandletLog" *> $null
"== commandlet exit=$LASTEXITCODE"
Select-String -Path $taskCommandletLog -Pattern 'OWT_DUPLICATION_VALIDATION' | Select-Object -Last 1 | ForEach-Object { $_.Line }
