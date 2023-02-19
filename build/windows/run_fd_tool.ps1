function Add-Path($Path) {
    $Path = [Environment]::GetEnvironmentVariable("PATH") + [IO.Path]::PathSeparator + $Path
    [Environment]::SetEnvironmentVariable( "Path", $Path )
}

Add-Path extras\curl\lib\release
Add-Path extras\jansson\lib\release
Add-Path extras\pcre\lib\release

Add-Path release\x64
.\release\x64\grassroots-fd-tool.exe
