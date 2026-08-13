$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$build = Join-Path $root "build"
$dist = Join-Path $root "dist"
$app = Join-Path $dist "app"

function Require-Command($name, $message) {
    if (-not (Get-Command $name -ErrorAction SilentlyContinue)) { throw $message }
}

Require-Command cmake "CMake is missing. Install Visual Studio 2022 with Desktop development with C++."
Require-Command git "Git is missing. Install Git for Windows."

$nsis = Get-Command makensis -ErrorAction SilentlyContinue
if (-not $nsis) {
    $candidate = "${env:ProgramFiles(x86)}\NSIS\makensis.exe"
    if (Test-Path $candidate) { $nsis = $candidate }
    else { throw "NSIS is missing. Download it from https://nsis.sourceforge.io/Download" }
}

New-Item -ItemType Directory -Force -Path $app | Out-Null
cmake -S $root -B $build -G "Visual Studio 17 2022" -A x64
cmake --build $build --config Release
Copy-Item (Join-Path $build "Release\FluidLab.exe") $app -Force

Get-ChildItem (Join-Path $build "Release") -Filter *.dll -ErrorAction SilentlyContinue |
    Copy-Item -Destination $app -Force

& $nsis (Join-Path $root "installer\FluidLab.nsi")
Write-Host ""
Write-Host "Installer created: $dist\FluidLab-Setup.exe" -ForegroundColor Green

