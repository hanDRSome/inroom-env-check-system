# Generic J-Link flash script: flash.ps1
# Locates build/Debug/<project>.elf from the workspace folder name,
# writes a temporary J-Link command script, then executes it.
# NOTE: keep this file ASCII-only so it parses under Windows PowerShell 5.1.
$ErrorActionPreference = 'Stop'

# Parent of .vscode is the project root
$workspace = Split-Path -Parent $PSScriptRoot
Set-Location $workspace

$project = Split-Path -Leaf $workspace
$elf = "build/Debug/$project.elf"

if (-not (Test-Path $elf)) {
    Write-Host "ERROR: $elf not found. Build first." -ForegroundColor Red
    exit 1
}

$jlink = Get-Command JLink.exe -ErrorAction SilentlyContinue
if (-not $jlink) {
    Write-Host "ERROR: JLink.exe not found in PATH." -ForegroundColor Red
    exit 1
}

$jlinkScript = @"
si SWD
speed 4000
device STM32F103C8
connect
loadfile $elf
r
g


"@

$scriptFile = Join-Path $workspace 'build/Debug/flash.jlink'
$jlinkScript | Set-Content -Path $scriptFile -Encoding ASCII

& JLink.exe -device STM32F103C8 -if SWD -speed 4000 -autoconnect 1 -ExitOnError 1 -CommanderScript $scriptFile
exit $LASTEXITCODE
