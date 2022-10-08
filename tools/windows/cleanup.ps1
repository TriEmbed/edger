# #Requires -RunAsAdministrator

function Test-Administrator  
{  
    [OutputType([bool])]
    param()
    process {
        [Security.Principal.WindowsPrincipal]$user = [Security.Principal.WindowsIdentity]::GetCurrent();
        return $user.IsInRole([Security.Principal.WindowsBuiltinRole]::Administrator);
    }
}

$idfdir = "$env:SystemDrive\Espressif"
$targetdir = "$env:userprofile\AppData\Local\edger"
$nvmdir = "$env:userprofile\AppData\Roaming\nvm"
$espinstaller = "$env:userprofile\Downloads\espressif-ide-setup-espressif-ide-2.5.0-with-esp-idf-4.4.exe"


if (Test-Path $idfdir) { Write-Host "1" ; Remove-Item -Recurse -Force $idfdir }
if (Test-Path $targetdir) { Write-Host "2" ; Remove-Item -Recurse -Force $targetdir }
if (Test-Path $nvmdir) { Write-Host "3" ; Remove-Item -Recurse -Force $nvmdir }
if (Test-Path "$env:APPDATA\npm") { Write-Host "4" ; Remove-Item -Recurse -Force "$env:APPDATA\npm" }
if (Test-Path $espinstaller) { Write-Host "5" ; Remove-Item -Force $espinstaller}

# This needs to run as Administrator
if (Test-Path "$env:ProgramFiles\nodejs") {
    if(-not (Test-Administrator))
    {
        Write-Host "Parts of this script require being Administrator"
        Write-Host "I will rerun this script as Administrator"
        Start-Sleep(5)
        Start-Process -Verb runas -PassThru -FilePath "powershell" -ArgumentList "$PSCommandPath" -Wait
        exit 0
    }

    # The three versions are trying to work around access denied error (probably fixed by being Administrator)
    if (Test-Path "$env:ProgramFiles\nodejs") { Write-Host "6" ; Remove-Item -Force"$env:ProgramFiles\nodejs" }
    if (Test-Path "$env:ProgramFiles\nodejs") { Write-Host "7" ; cmd /c rmdir "$env:ProgramFiles\nodejs" }
    if (Test-Path "$env:ProgramFiles\nodejs") { Write-Host "8" ; [System.IO.Directory]::Delete("$env:ProgramFiles\nodejs") }
}




