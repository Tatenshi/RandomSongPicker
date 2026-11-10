Param(
    [Parameter(Mandatory=$false)]
    [Switch] $clean,

    [Parameter(Mandatory=$false)]
    [Switch] $help
)

if ($help -eq $true) {
    echo "`"Build`" - Copiles your mod into a `".so`" or a `".a`" library"
    echo "`n-- Arguments --`n"

    echo "-Clean `t`t Deletes the `"build`" folder, so that the entire library is rebuilt"
    exit
}


# Check if ./extern/includes/bs-cordl/version.txt exists
if (Test-Path "$PSScriptRoot/extern/includes/bs-cordl/include/version.txt") {
    # Update packageVersion in mod.template.json using bs-cordl version.txt
    $modTemplateRaw = Get-Content "$PSScriptRoot/mod.template.json" -Raw
    $modTemplateOriginal = $modTemplateRaw | ConvertFrom-Json
    $modTemplate = $modTemplateRaw | ConvertFrom-Json
    $bsversion = Get-Content "$PSScriptRoot/extern/includes/bs-cordl/include/version.txt"
    if (-not [string]::IsNullOrWhitespace($bsversion)) {
        Write-Output "Setting Package Version to $bsversion"
        $modTemplate.packageVersion = $bsversion

        # Write the updated mod.template.json if the contents have changed
        if (($modTemplate | ConvertTo-Json -Depth 32) -ne ($modTemplateOriginal | ConvertTo-Json -Depth 32)) {
            Write-Output "Writing updated mod.template.json"
            $modTemplate | ConvertTo-Json -Depth 32 | Set-Content -Path "$PSScriptRoot/mod.template.json"
        }
    }
    else {
        Write-Output "Empty bs-cordl version.txt, skipping package version update."
    }
}
else {
    Write-Output "Missing bs-cordl version.txt, skipping package version update."
}

# if user specified clean, remove all build files
if ($clean.IsPresent)
{
    if (Test-Path -Path "build")
    {
        remove-item build -R
    }
}


if (($clean.IsPresent) -or (-not (Test-Path -Path "build")))
{
    $out = new-item -Path build -ItemType Directory
} 

& cmake -G "Ninja" -DCMAKE_BUILD_TYPE="RelWithDebInfo" -B build
& cmake --build ./build
$ExitCode = $LastExitCode
exit $ExitCode
