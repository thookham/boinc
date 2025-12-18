<#
.SYNOPSIS
    Executes unit tests for BOINC on Windows.
.DESCRIPTION
    This script attempts to find the compiled unit tests executable and run it.
    It looks in standard Visual Studio output directories.
#>

$TestDirs = @(
    "win_build/Build/Debug",
    "win_build/Build/Release",
    "win_build/x64/Debug",
    "win_build/x64/Release"
)

$UnitTestExe = "unittests.exe"
$Found = $false

foreach ($Dir in $TestDirs) {
    # Construct path relative to script location
    $Path = Join-Path -Path $PSScriptRoot -ChildPath ("..\" + $Dir + "\" + $UnitTestExe)
    if (Test-Path $Path) {

        Write-Host "Found unit tests at: $Path"
        $Found = $true
        & $Path --gtest_output=xml:test_report.xml
        break
    }
}

if (-not $Found) {
    Write-Warning "Could not find $UnitTestExe. Please build the 'unittests' project in 'win_build/boinc.sln' first."
    exit 1
}
