#!/usr/bin/env pwsh
# TriEmbed ESP32/Dialog FPGA Project
#
# Tool chain installation script

# This is the Windows port of the linux installit.sh script
# Based on version="0.32"

# See https://github.com/TriEmbed/que_tools/issues for bugs/enhancements

# Authors and maintainers
# Add your email here and append your name to the copyright list with commas

# pete@soper.us
# rob.mackie@gmail.com
# nickedgington@gmail.com
# paulmacd@acm.org

# MIT License
#
# Copyright 2022 Dawn Annette Trembath, Peter James Soper, Robert Andrew Mackie, Nicholas John Edgington, Paul Duncan MacDougal
#
# Permission is hereby granted, free of charge, to any person 
# obtaining a copy of this software and associated 
# documentation files (the "Software"), to deal in the
# Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, 
# distribute, sublicense, and/or sell copies of the
# Software, and to permit persons to whom the Software is 
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall
# be included in all copies or substantial portions of the 
# Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY 
# KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
# OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Param (
    [Parameter(Position = 0)][ValidateNotNullOrEmpty()][string] $WIFISSID,
    [Parameter(Position = 1)][ValidateNotNullOrEmpty()][string] $WIFIpassword,
    # The default target ESP32 device
    [ValidateSet('ESP32', 'ESP32C3', 'ESP32S2')][string] $targetdevice = 'ESP32C3',
    # The directory pathname for the user's Edger repositories
    [string]$targetdir = "$env:userprofile\AppData\Local\edger",
    # The base part of the branch name shown by git branch -a
    [string]$targetbranch = 'origin/release/v4.4',
    # The default version of que_purple board.
    [ValidateSet('60', '70')][string]$c3board = '70',
    [string]$branch,
    [switch]$version,
    [switch]$help,
    [switch]$talkative
)

# Bump this per push
$versionId = '0.1'

# capture the tools directory (Assumes that this script has not been moved since cloned)
#temp=`dirname $0`
#rootpath=`readlink -f $temp`

# The directory pathname for the Espressif IDF directory
# Note this cannot be currently changed from the command line. 
# Be careful about having multiple instances of esp-idf because the most
# recent invocation of install.sh governs which is used, NOT which one contained
# the export.sh file that was sourced. (Is this completely correct? It is for
# sure partially correct 'cuase install.sh mutates ~/.expressif but export.sh
# does not).
$idfdir = "$env:SystemDrive\Espressif"

# The git branch labels for the exact version of tools to install. If this 
# variable does not match the branch of an existing esp-idf the script must
# be aborted because there is the likelihood that the submodules are not right.
# The required order is 1) clone, 2) checkout branch, 3) load submodules
# The remotes/origin/release/v4.4 branch is the latest Espressif stable branch.
# the remotes/origin/release/v5.0 branch is the bleeding edge development 
# branch. 

# Git is weird with respect to git branch -a output vs actual branch names
# The correct branch name is $targetbranchprefix$targetbranch
# $targetbranchprefix = 'remotes/'

# The nvm version
$nodeversion = '14.20.0'
$nvmversion = '1.1.9'
$npmversion = '6.14.17'

# $MyInvocation.MyCommand.Path
# $MyInvocation.MyCommand.Name

function usage {
    #echo $1
    Write-Output 'usage: installit WIFISSID WIFIpassword [ -targetdevice <ESP32 | ESP32C3 | ESP32S2> ] [ -targetdir <path> ] [ -branch <branch id> ] [ -c3board <60 | 70> ] [ -version ] [ -help ]'
    Write-Output "default targetdir: $targetdir"
    Write-Output "default branch: $targetbranch"
    Write-Output "default device: $targetdevice"
    Exit 1
}

function myhelp {
    Write-Output 'ESP32, ESP32S3 and ESP32C3 supported'
    Write-Output 'I2C pins as follows:'
    Write-Output 'ESP32: SDA 18 SCL 19'
    Write-Output 'ESP32S2: SDA 1 SCL 0'
    Write-Output 'ESP32C3: SDA 18 SCL 19 (m80 60 rev) SDA 1 SCL 0 (m80 70 rev)'
    usage
}

# Put out a prompt, then ask for a Y, y, N or n response.
# Return true if Y or y, false if N or n. Cannot escape until an acceptable 
# answer is input. Use ctrl-C to break out.

function getyes {
    Param($prompt)
    while ($True) {
        $answer = Read-Host -Prompt $prompt
        if ($answer -eq 'y' -Or $answer -eq 'Y') {
            RETURN $True
        }
        if ($answer -eq 'n' -Or $answer -eq 'N') {
            RETURN $False
        }
        Write-Host 'Answer MUST be Y, y, N, or n'
    }
}

function Test-CommandExists {
    Param($command)
    $oldPreference = $ErrorActionPreference
    $ErrorActionPreference = 'stop'
    try {
        if (Get-Command $command) {
            # may cause non-terminating error
            # command is installed
            # Write-Output "$command is installed"
            return $True
        }
    }
    Catch {
        # Write-Output "$command is not installed"
        return $False
    }
    Finally { $ErrorActionPreference = $oldPreference }
}

function makeDirectory {
    Param($dirname)

    if (!(Test-Path $dirname)) {
        # offer to create the directory
        if (getyes "Would you like me to create $dirname for you?") {
            $oldPreference = $ErrorActionPreference
            $ErrorActionPreference = 'stop'
            try {
                New-Item $dirname -ItemType Directory | Out-Null # suppress directory creation spew
            }
            Catch [System.UnauthorizedAccessException] {
                Write-Error -Category PermissionDenied -ErrorId 'E04' -Message "Permission to create $dirname is denied"
                Exit 4
            }
            Catch {
                # This is an interesting case that we would like to include in our testing.  At the moment, we do not have an example of hitting this.
                Write-Error -Category PermissionDenied -ErrorId 'E05' -Message "Failed to create $dirname `r`n${_.Exception.Message}`r`n"
                Exit 5
            }
            Finally { $ErrorActionPreference = $oldPreference }
        }
        else {
            # Do we want to write out any message here?
            Exit 5
        }
    }
    elseif (Test-Path $dirname -PathType Leaf) {
        # a file by that name exists.  What to do?
        Write-Error -Category ResourceExists -ErrorId 'E06' -Message `
            "This script would like to create a directory at $dirname `
    A file by that name is already present.  Either change `
    the -targetdir parameter or remove the existing file `
    and rerun this script"
        Exit 6
    }
}

# Switch parameter processing
if ($version) {
    Write-Output "version: $versionId"
    Exit 0
}

if ($help) {
    myhelp # this calls usage, which exits
    Exit 1 # we never get here
}

# Additional parameter checking
$exit = $False
if ($WIFISSID -eq '') {
    Write-Error -Category InvalidArgument -ErrorId 'E01' -Message 'The WiFi SSID parameter must be present.'
    # exit the script after doing parameter error checking
    $exit = $True
}

if ($WIFIpassword -eq '') {
    Write-Error -Category InvalidArgument -ErrorId 'E02' -Message 'The WiFi passphrase parameter must be present.'
    # exit the script after doing parameter error checking
    $exit = $True
}

if ($exit) {
    Exit 2 # available in the invoking shell in $lastexitcode
}

# Configure I2C pins based on processor selected
if ($targetdevice -eq 'ESP32C3' -And $c3board -eq '60') {
    $targetsda = 1
    $targetscl = 0
}
elseif ($targetdevice -eq 'ESP32C3' -And $c3board -eq '70') {
    $targetsda = 18
    $targetscl = 19
}
elseif ($targetdevice -eq 'ESP32') {
    $targetsda = 18
    $targetscl = 19
}
elseif ($targetdevice -eq 'ESP32S2') {
    $targetsda = 1
    $targetscl = 0
}
else {
    Write-Error -Category NotImplemented -ErrorId 'E03' -Message "Write code to handle $targetdevice with $c3board"
    Exit 3
}

Write-Host '1'
Get-Date -DisplayHint DateTime

# We assume that the entire edger repo has been downloaded
# and we are running this script from there
if (!((Test-Path "$PSScriptRoot\..\..\aardvark" -PathType Container) `
 -And (Test-Path "$PSScriptRoot\..\..\ant" -PathType Container)))
{
    Write-Error -Category InvalidOperation -ErrorId 'E15' -Message "
    This script assumes that it is being run from a downloaded edger repo.
    If this were true, there would be aardvark and ant directories nearby.
    I did not find $PSScriptRoot\..\..\aardvark and $PSScriptRoot\..\..\ant"
    Exit 15
}

# install ESP-IDF
if ((Test-Path "$idfdir\frameworks\esp-idf-v4.4" -PathType Container) `
 -And (Test-Path "$idfdir\tools\espressif-ide\2.5.0\espressif-ide.exe" -PathType Leaf) `
 -And (Test-Path "$idfdir\Initialize-Idf.ps1" -PathType Leaf)) {
        # All is good
        if ($talkative) { Write-Host 'ESP-IDF appears to be properly installed'}
} else {
    if ($talkative) { Write-Host 'About to install ESP-IDF'}
    # Reinstall ESP-IDF

    # The easiest way to install ESP-IDF’s prerequisites is to download one of ESP-IDF Tools Installers
    # from this URL: https://dl.espressif.com/dl/esp-idf/?idf=4.4
    # https://dl.espressif.com/dl/idf-installer/espressif-ide-setup-espressif-ide-2.5.0-with-esp-idf-4.4.exe
    if (!(Test-Path "$env:userprofile\Downloads\espressif-ide-setup-espressif-ide-2.5.0-with-esp-idf-4.4.exe" -PathType Leaf)) {
        $oldPreference = $ErrorActionPreference
        $ErrorActionPreference = 'stop'
        Try {
            if ($talkative) { Write-Host 'Downloading the ESP-IDF installation program.  This can take several minutes to download the 973,299,888 bytes.' }
            Write-Host '2'
            Get-Date -DisplayHint Time
            $source = 'https://dl.espressif.com/dl/idf-installer/espressif-ide-setup-espressif-ide-2.5.0-with-esp-idf-4.4.exe'
            $destination = "$env:userprofile\Downloads\espressif-ide-setup-espressif-ide-2.5.0-with-esp-idf-4.4.exe"
            Invoke-RestMethod -Uri $source -OutFile $destination
        }
        Catch [System.UnauthorizedAccessException] {
            Write-Error -Category PermissionDenied -ErrorId 'E11' -Message "Failed to download ESP-IDF installer`n$_"
            Exit 11
        }
        Catch {
            $message = $_.Exception
            Write-Error -Category PermissionDenied -ErrorId 'E12' -Message "Failed to download ESP-IDF installer`r`n$message`r`n"
            Exit 12            
        }
        Finally { $ErrorActionPreference = $oldPreference }

        # ToDo: check that this worked ...
        if ($talkative) { Write-Host "Download ESP-IDF installer status: $lastexitcode" }
        # 9009 System.UnauthorizedAccessException
    }

    Write-Host '3'
    Get-Date -DisplayHint Time
    Write-Host "We are now going to install the ESP-IDF toolset.
    You should step through the dialogs, accepting the defaults and
    advancing through the various steps.  It will take a few minutes.
    You may un-check the box that says to always trust files from Espressif if you like."
    Start-Process -Wait -FilePath "$env:userprofile\Downloads\espressif-ide-setup-espressif-ide-2.5.0-with-esp-idf-4.4.exe"
    # ToDo: check that this worked ...
    
    if ((Test-Path "$idfdir\frameworks\esp-idf-v4.4" -PathType Container) `
    -And (Test-Path "$idfdir\tools\espressif-ide\2.5.0\espressif-ide.exe" -PathType Leaf) `
    -And (Test-Path "$idfdir\Initialize-Idf.ps1" -PathType Leaf)) {
        if ($talkative) { Write-Host 'ESP-IDF installation worked' }
        # do we want to touch a DOTfile?
     } else {
        # Failure of some sort
        Write-Error -Category NotInstalled -ErrorId 'E08' "espressif-ide-setup-espressif-ide-2.5.0-with-esp-idf-4.4.exe did not create the expected files:
            $idfdir\frameworks\esp-idf-v4.4
            $idfdir\tools\espressif-ide\2.5.0\espressif-ide.exe
            $idfdir\Initialize-Idf.ps1
            "
        Exit 8
    }
}

# Create directories if they do not already exist
Write-Host '4'
Get-Date -DisplayHint Time
makeDirectory $targetdir
Push-Location $targetdir

if (!(Test-Path "$targetdir\que_aardvark" -PathType Container)) {
    if ($talkative) { Write-Host 'copy aardvark to que_aardvark'}
    # ToDo: Try/Catch here
    Copy-Item -Recurse -Force -Path "$PSScriptRoot\..\..\aardvark" -Destination "$targetdir\que_aardvark"
    # ToDo: success check here
}

if (!(Test-Path "$targetdir\que_ant" -PathType Container)) {
    if ($talkative) { Write-Host 'copy ant to que_ant'}
    # ToDo: Try/Catch here
    Copy-Item -Recurse -Force -Path "$PSScriptRoot\..\..\ant" -Destination "$targetdir\que_ant"
    # ToDo: success check here
}

# install nvm
$installNvm = $False
if (Test-Path "$env:APPDATA\nvm\nvm.exe" -PathType Leaf) {
    if (. "$env:APPDATA\nvm\nvm.exe" version | Select-String -SimpleMatch -Pattern "$nvmversion") {
        if ($talkative) { Write-Host "nvm.exe version $nvmversion is currently installed" }
    } else {
        $installNvm = True
    }

    if (. "$env:APPDATA\nvm\nvm.exe" list | Select-String -SimpleMatch -Pattern "$nodeversion") {
        if ($talkative) { Write-Host "Node.js version $nodeversion is currently installed" }
    } else {
        $installNvm = $True
    }
} else {
    $installNvm = $True
}

Write-Host '5'
Get-Date -DisplayHint Time
if ($installNvm) {
    if ($talkative) { Write-Host 'install nvm'}
    if (!(Test-Path "$env:APPDATA\nvm" -PathType Container)) {
        # remove existing %ProgramFiles%\nodejs
        # Check with user that this is OK to do here
        if (Test-Path "$env:ProgramFiles\nodejs" -PathType Container) {
            # ToDo: Need a try/Catch here
            Remove-Item -Force"$env:ProgramFiles\nodejs"
            #cmd /c rmdir "$env:ProgramFiles\nodejs"
            #[System.IO.Directory]::Delete("$env:ProgramFiles\nodejs",$true)
        }

        # backup %APPDATA%\npm\etc\npmrc
        if (Test-Path "$env:APPDATA\npm\etc\npmrc" -PathType Leaf) {
            # ToDo: Need a try/Catch here
            Copy-Item "$env:APPDATA\npm\etc\npmrc" "$targetdir\npmrc_backup"
        }

        # remove %APPDATA%\npm
        if (Test-Path "$env:APPDATA\npm" -PathType Container) {
            # ToDo: Need a try/Catch here
            Remove-Item -Recurse -Force "$env:APPDATA\npm"
        }

        # download: nvm-setup.zip
        $Url = 'https://github.com/coreybutler/nvm-windows/releases/download/1.1.9/nvm-setup.zip'
        $DownloadZipFile = "$targetdir\" + $(Split-Path -Path $Url -Leaf)
        if (!(Test-Path $DownloadZipFile -PathType Leaf)) {
            Write-Host '6'
            Get-Date -DisplayHint Time
            Write-Host 'About to download the NVM installer'

            Invoke-WebRequest -Uri $Url -OutFile $DownloadZipFile
            if (!(Test-Path $DownloadZipFile -PathType Leaf)) {
                Write-Error -Category NotInstalled -ErrorId 'E14' "Failed to download $DownloadZipFile from github"
                Exit 14
            }
        }

        # Extract setup file from the zip file
        if (!(Test-Path "$targetdir\nvm_zip\" -PathType Container)) {
            makeDirectory("$targetdir\nvm_zip\")
            # What do we do if user says No?
        }

        if (!(Test-Path "$targetdir\nvm_zip\nvm-setup.exe" -PathType Leaf)) {        
            $ExtractShell = New-Object -ComObject Shell.Application
            $zipPackage = $ExtractShell.NameSpace($DownloadZipFile)
            $listOfItems = $zipPackage.Items()
            $ExtractShell.NameSpace("$targetdir\nvm_zip\").CopyHere($listOfItems)
            #Start-Process $ExtractShell -Wait
            # ToDo: check for success
        }

        if (!(Test-Path "$targetdir\nvm_zip\nvm-setup.exe" -PathType Leaf)) {
            # Exit the script
            Write-Error -Category NotInstalled -ErrorId 'E13' "Failed to extract $targetdir\nvm_zip\nvm-setup.exe from 
            $env:APPDATA\nvm\nvm.exe
            "
            Exit 13
        }

        # run $targetdir\nvm_zip\nvm-setup.exe
        Write-Host '7'
        Get-Date -DisplayHint Time
        Write-Host 'About to run the NVM installer'
        Write-Host 'This will pop up a window to request permission for `"NVM for Windows`"'
        Write-Host 'Allow this to make changes and accept the default settings'
        Start-Sleep(5) # allow user to read the prompt
        Start-Process -FilePath "$targetdir\nvm_zip\nvm-setup.exe" -Wait # -WorkingDirectory 'foo'

        if (!(Test-Path "$env:APPDATA\nvm\nvm.exe" -PathType Leaf)) {
            # installation failure ...
            Write-Error -Category NotInstalled -ErrorId 'E09' "$targetdir\nvm_zip\nvm-setup.exe did not create the expected files:
                $env:APPDATA\nvm\nvm.exe
                "
            Exit 9
        }
    }

    if (. "$env:APPDATA\nvm\nvm.exe" list | Select-String -SimpleMatch -Pattern "$nodeversion") {
        if ($talkative) { Write-Host "Node.js version $nodeversion is currently installed" }
    } else {
        Write-Host '8'
        Get-Date -DisplayHint Time
        Write-Host 'About to install node.js'
        Write-Host 'Accept the popup that asks for permission for `'nvm.exe`"''
        Start-Sleep(5) # allow user to read the prompt
        if ($talkative) { Write-Host "install node.js version $nodeversion" }
        #Start-Process -FilePath "$env:APPDATA\nvm\nvm.exe" -ArgumentList "install $nodeversion" -Wait -PassThru

        # $env:NVM_HOME = "$env:APPDATA\nvm"
        # should we use . or & here?
        & "$env:APPDATA\nvm\nvm.exe" install $nodeversion

        # This is what we are expecting to happen
        #PS C:\Users\Paul\AppData\Roaming\nvm> nvm install 14
        #Downloading node.js version 14.20.0 (64-bit)...
        #Complete
        #Creating C:\Users\Paul\AppData\Roaming\nvm\temp
        #
        #Downloading npm version 6.14.17... Complete
        #Installing npm v6.14.17...
        #
        #Installation complete. If you want to use this version, type
        #
        #nvm use 14.20.0    
    }
}

$cmdoutput = & $env:APPDATA\nvm\nvm.exe current
if ($cmdoutput  | Select-String -SimpleMatch -Pattern "$nodeversion") {
    # all is good
    if ($talkative) { Write-Host 'Proper version of node.js is being used'}
} elseif ($cmdoutput | Select-String -SimpleMatch -Pattern 'No current version.') {
    # need to do a "use"
    Write-Host '9'
    Get-Date -DisplayHint Time
    Write-Host "Need to `"use`" node.js version $nodeversion"
    Write-Host 'This will pop up a window to request permission for `"nvm.exe`"'
    Write-Host 'Allow this to make changes'
    Start-Sleep(5) # allow user to read the prompt

    # this requires running as Administrator
    Start-Process -Verb runas -PassThru -FilePath "$env:APPDATA\nvm\nvm.exe" -ArgumentList "use","$nodeversion" -Wait
    # ToDo: how do we validate that this worked?
    $cmdoutput = & $env:APPDATA\nvm\nvm.exe current
    if ($cmdoutput  | Select-String -SimpleMatch -Pattern "$nodeversion") {
        # all is good
    } else {
        Write-Error -ErrorId 'E10' -Category NotImplemented "Need code to handle `"nvm.exe use $nodeversion`" failure"
        Exit 10
    }

} else {
    Write-Error -ErrorId 'E11' -Category NotImplemented "Need code to handle `"nvm.exe current`" output: $cmdoutput"
    Exit 11
}

$cmdoutput = & $env:APPDATA\nvm\v14.20.0\npm.cmd --version
if ($cmdoutput | Select-String -SimpleMatch -Pattern $npmversion) {
    # all is good
    if ($talkative) { Write-Host 'Proper version of npm is being used'}
} else {
    Write-Error -ErrorId 'E12' -Category NotImplemented "Need code to handle `"npm.cmd --version`" output: $cmdoutput"
    Exit 12
}

if ($null -eq $env:IDF_TOOLS_PATH) {
    $env:IDF_TOOLS_PATH="$idfdir"
    if ($talkative) { Write-Host "IDF_TOOLS_PATH not set. Setting to $idfdir"}
}

$IdfId = 'esp-idf-8de2bd0d9cffd2eca3d3f8442939a034'
$PythonCommand = & "$idfdir\idf-env.exe" config get --property python --idf-id ${IdfId}
#$IDF_PATH = & "$idfdir\idf-env.exe" config get --property path --idf-id ${IdfId}

# Clear PYTHONPATH as it may contain libraries of other Python versions
$env:PYTHONPATH=$null

# Clear PYTHONHOME as it may contain path to other Python versions which can cause crash of Python using virtualenv
$env:PYTHONHOME=$null

# Set PYTHONNOUSERSITE to avoid loading of Python packages from AppData\Roaming profile
if ($null -eq $env:PYTHONNOUSERSITE) {
    Write-Host 'Setting PYTHONNOUSERSITE, was not set'
    $env:PYTHONNOUSERSITE="True"
}

# Strip quotes
$IdfPythonDir = (Get-Item $PythonCommand).Directory.FullName

# Add Python path to PATH
$env:PATH = "$IdfPythonDir;$env:PATH"
#"Using Python in $IdfPythonDir"
#&$PythonCommand --version

# go to que_aardvark and build

Push-Location "$targetdir\que_aardvark\src"
#f1=`esptool.py chip_id | grep MAC: | head -1 | cut -d: -f 6`
#f2=`esptool.py chip_id | grep MAC: | head -1 | cut -d: -f 7`
#if [ $? -ne 0 ] ; then
#  echo "dev board not connected: using 0000 as MAC address low order digits"
#  f1="00"
#  f2="00"
#fi
#echo "export default {" >autoconfiguration.js
##echo "  localurl:  'http://ant_$f1$f2.local'" >>autoconfiguration.js
#echo "  localurl:  'http://ant_0000.local'" >>autoconfiguration.js
#echo "}" >>autoconfiguration.js
##echo "micro DNS url for ant: http://ant_$f1$f2.local"
#echo "micro DNS url for ant: http://ant_0000.local"
Pop-Location

Push-Location "$targetdir\que_aardvark"
    Write-Host '10'
    Get-Date -DisplayHint Time
    Write-Host 'About to build que_aardvark'
    Start-Sleep(5)
    if (!(Test-Path '.installed' -PathType Leaf)) {
        if ($talkative) { Write-Host 'installing npm in que_aardvark' }
        $cmdoutput = & npm install
        # ToDo: validate successful run here
        # Write-Output "npm install status: $lastexitcode"
        New-Item -Path '.installed' | Out-Null
    }

    if ($talkative) { Write-Host 'building npm in que_aardvark' }
    $cmdoutput = & npm run build
    # ToDo: validate successful run here
    Write-Output "que_aardvark npm run build status: $lastexitcode"
Pop-Location

Push-Location "$targetdir\que_ant\ant"
    Write-Host '11'
    Get-Date -DisplayHint Time
    Write-Host 'About to build que_ant'
    Start-Sleep(5)
    if (!(Test-Path '.installed' -PathType Leaf)) {
        if ($talkative) { Write-Host 'installing npm in que_ant' }
        $cmdoutput = & npm install
        # ToDo: validate successful run here
        # Write-Output "npm install status: $lastexitcode"
        New-Item -Path '.installed' | Out-Null
    }

    Write-Host '12'
    Get-Date -DisplayHint Time
    if ($talkative) { Write-Host 'building npm in que_ant' }
    $cmdoutput = & npm run build
    # ToDo: validate successful run here
    Write-Output "que_ant npm run build status: $lastexitcode"
Pop-Location

Write-Output 'Kconfig.projbuild edits in que_ant'
Push-Location "$targetdir\que_ant\components\apsta"
    Write-Host '13'
    Get-Date -DisplayHint Time
    (Get-Content Kconfig.projbuild -Raw) -replace 'default.*# ROUTERSSID', "default `"$WIFISSID`" # ROUTERSSID" | Set-Content Kconfig.projbuild
    (Get-Content Kconfig.projbuild -Raw) -replace 'default.*# ROUTERPASSWORD', "default `"$WIFIpassword`" # ROUTERPASSWORD" | Set-Content Kconfig.projbuild
    #sed -e"s+.*ROUTERSSID.*$+               default $targetSSID   # ROUTERSSID+" Kconfig.projbuild >/tmp/tmp.$$
    #sed -e"s+.*ROUTERPASSWORD.*$+               default $targetpassword   # ROUTERPASSWORD+" /tmp/tmp.$$ >Kconfig.projbuild
Pop-Location

Push-Location "$targetdir\que_ant\main"
    Write-Host '14'
    Get-Date -DisplayHint Time
    (Get-Content Kconfig.projbuild -Raw) -replace 'default.*# TARGETSDA', "default $targetsda # TARGETSDA" | Set-Content Kconfig.projbuild
    (Get-Content Kconfig.projbuild -Raw) -replace 'default.*# TARGETSCL', "default $targetscl # TARGETSCL" | Set-Content Kconfig.projbuild
    #sed -e"s+.*TARGETSDA.*$+		default $targetsda   # TARGETSDA+" Kconfig.projbuild >/tmp/tmp.$$
    #sed -e"s+.*TARGETSCL.*$+		default $targetscl   # TARGETSCL+" /tmp/tmp.$$ >Kconfig.projbuild
Pop-Location


Push-Location "$targetdir\que_ant"
    Write-Host '15'
    Get-Date -DisplayHint Time
    #$env:IDF_PYTHON_ENV_PATH = "$idfdir\python_env\idf4.4_py3.8_env"
    . "$idfdir\frameworks\esp-idf-v4.4\export.ps1" | Out-Host # change to OUT-Null

    if (Test-Path build -PathType Container) {
        Write-Host '16'
        Get-Date -DisplayHint Time
        Remove-Item -Recurse -Force 'build'
    }

    Write-Host '17'
    Get-Date -DisplayHint Time
    $cmdoutput = & idf.py set-target "$targetdevice" | Out-Host # change to OUT-Null
    # ToDo: validate successful run here

    Write-Host '18'
    Get-Date -DisplayHint Time
    $cmdOutput = & "idf.py" build | Out-Host # change to OUT-Null
    # ToDo: validate successful run here
Pop-Location

Write-Host '19'
Get-Date -DisplayHint Time

# May not have these env variables, though
# NVM_HOME                       C:\Users\Paul\AppData\Roaming\nvm
# NVM_SYMLINK                    C:\Program Files\nodejs


$date = Get-Date -format 'yyyy-MM-dd'
#Clear-Host
Write-Output '\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/'
Write-Output "Welcome $Env:USERNAME"
Write-Output $date
Write-Output 'Edger installation script'

Write-Output "$PSScriptRoot" # C:\Users\Paul\Dropbox\Edger\que_tools\windows
Write-Output "$PSCommandPath" # C:\Users\Paul\Dropbox\Edger\que_tools\windows\test.ps1
Write-Output $(Split-Path -Path $PSCommandPath -Leaf) # test.ps1

Write-Output "targetdir: $targetdir"
Write-Output "idfdir: $idfdir"
Write-Output "targetdevice: $targetdevice"
Write-Output "targetbranch: $targetbranch"#
Write-Output "nvm version: $nodeversion"
Write-Output "npm version: $npmversion"
Write-Output "targetsda: $targetsda"
Write-Output "targetscl: $targetscl"
Write-Output "targetSSID: $WIFISSID"
Write-Output "targetpassword: $WIFIpassword"

# List PowerShell's Environmental Variables
#Get-Childitem -Path Env:* | Sort-Object Name


# $windows_path = $env:Path
# $windows_path -split ';'

Write-Host '20'
Get-Date -DisplayHint Time
Write-Host 'The End'
