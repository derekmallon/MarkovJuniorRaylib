Set-Location -Path 'C:\Users\Derek\MarkovJuniorRaylib'
$Host.UI.RawUI.WindowTitle = 'Debugger'
ninja
lldb -o run .\markov.exe
Write-Host "press any key to close" -ForegroundColor Yellow
$x = $host.ui.RawUI.ReadKey("NoEcho,IncludeKeyDown")

