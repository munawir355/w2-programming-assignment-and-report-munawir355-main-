Push-Location
try {
    .\build.ps1 $args[0]  
}
catch {
    Write-Host "Something crashed"
}
finally {
    Write-Host "Exiting"
    Pop-Location
}

