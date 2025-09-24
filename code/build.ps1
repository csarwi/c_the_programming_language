param(
    [switch]$shipping
)

# Create build folder (next to /code) if it doesn't exist
$Root = Split-Path $PSScriptRoot -Parent
$Build = Join-Path $Root "build"
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
        # Shipping/Release build
        $Result = & cl -nologo -O2 -W3 -TC "$SrcFile" -Fe:"$OutFile" 2>&1
    }
    else {
        # Debug build
        $Result = & cl -nologo -Zi -Od -W3 -TC "$SrcFile" -Fe:"$OutFile" 2>&1
    }
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "  Success" -ForegroundColor Green
        $SuccessCount++
    }
    else {
        Write-Host "  Failed" -ForegroundColor Red
        Write-Host $Result
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