<#
.SYNOPSIS
    Counts the number of lines in each file within a specified directory.

.PARAMETER Path
    The directory to scan.

.PARAMETER Recurse
    Optional switch to include subdirectories.

.EXAMPLE
    .\Count-Lines.ps1 -Path "C:\Projects\MyApp"
    .\Count-Lines.ps1 -Path "C:\Projects\MyApp" -Recurse
#>

param(
    [Parameter(Mandatory = $true)]
    [string]$Path,

    [switch]$Recurse
)

if (-not (Test-Path -Path $Path -PathType Container)) {
    Write-Error "Error: '$Path' is not a valid directory"
    exit 1
}

$getFilesParams = @{
    Path = $Path
    File = $true
}
if ($Recurse) {
    $getFilesParams["Recurse"] = $true
}

$files = Get-ChildItem @getFilesParams

$totalLines = 0
$results = foreach ($file in $files) {
    $lineCount = (Get-Content -Path $file.FullName | Measure-Object -Line).Lines
    $totalLines += $lineCount

    [PSCustomObject]@{
        FileName  = $file.Name
        LineCount = $lineCount
    }
}

$results | Format-Table -AutoSize

Write-Host "----------------------------------------"
Write-Host "Total lines: $totalLines"