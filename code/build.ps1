param(
    [switch]$shipping
)

# Create build folder (next to /code) if it doesn't exist
$Root = Split-Path $PSScriptRoot -Parent
$Build = Join-Path $Root "build"

# Clean build folder - remove all files
if (Test-Path $Build) {
    Write-Host "Cleaning build directory..."
    Remove-Item -Path "$Build\*" -Recurse -Force -ErrorAction SilentlyContinue
}
else {
    Write-Host "Creating build directory..."
}
New-Item -ItemType Directory -Force -Path $Build | Out-Null

# Go into /build
Push-Location $Build

# Print working directory
Write-Host "Building in: $(Get-Location)"

# Find all .c files in the code directory
$CodeDir = Join-Path $Root "code"
$CFiles = Get-ChildItem -Path $CodeDir -Filter "*.c"

if ($CFiles.Count -eq 0) {
    Write-Host "No .c files found in $CodeDir"
    Pop-Location
    exit 1
}

Write-Host "Found $($CFiles.Count) C file(s) to compile:"
$CFiles | ForEach-Object { Write-Host "  - $($_.Name)" }
Write-Host ""

$SuccessCount = 0
$FailureCount = 0

# Compile each .c file into its own executable
foreach ($CFile in $CFiles) {
    $BaseName = [System.IO.Path]::GetFileNameWithoutExtension($CFile.Name)
    $OutFile = Join-Path $Build "$BaseName.exe"
    $SrcFile = $CFile.FullName
    
    Write-Host "Compiling $($CFile.Name) -> $BaseName.exe"
    
    if ($shipping) {
        $Result = & cl -nologo -O2 -W4 -Wall -TC "$SrcFile" -Fe:"$OutFile" 2>&1
    }
    else {
        $Result = & cl -nologo -Zi -Od -W4 -Wall -TC "$SrcFile" -Fe:"$OutFile" 2>&1
    }
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "  Success" -ForegroundColor Green
        $SuccessCount++
    }
    else {
        Write-Host "  Failed" -ForegroundColor Red
        Write-Host ""
        
        if ($Result) {
            $ErrorLines = $Result -split "`n" | Where-Object { $_.Trim() -ne "" }
            foreach ($Line in $ErrorLines) {
                $TrimmedLine = $Line.Trim()
                if ($TrimmedLine -match "error\s+C\d+:") {
                    Write-Host "    $TrimmedLine" -ForegroundColor Red
                }
                elseif ($TrimmedLine -match "warning\s+C\d+:") {
                    Write-Host "    $TrimmedLine" -ForegroundColor Yellow
                }
                elseif ($TrimmedLine -match "note:") {
                    Write-Host "    $TrimmedLine" -ForegroundColor Cyan
                }
                elseif ($TrimmedLine -ne "") {
                    Write-Host "    $TrimmedLine" -ForegroundColor Gray
                }
            }
        }
        
        $FailureCount++
    }
    Write-Host ""
}

# Summary
Write-Host "=== Build Summary ==="
Write-Host "Successful: $SuccessCount" -ForegroundColor Green
Write-Host "Failed: $FailureCount" -ForegroundColor Red
Write-Host "Total: $($CFiles.Count)"

# Return to original location
Pop-Location