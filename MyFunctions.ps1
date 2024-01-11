function delIfExists( $fn )
{
    if (Test-Path -Path $fn) {
        Remove-Item -Force -Recurse $fn
    }
}
function textWidth( $text )
{
    $LEN = $text.Length
    $MAX_WIDTH = 0
    $WIDTH = 0
    for ($n = 0; $n -le $LEN; $n++) {
        $nextChar = $text[$n]
        if ($nextChar -eq "`n") {
            if ($WIDTH -gt $MAX_WIDTH) {
                $MAX_WIDTH = $WIDTH
            }
            $WIDTH = 0
        } else {
            $WIDTH++
            if ($WIDTH -gt $MAX_WIDTH) {
                $MAX_WIDTH = $WIDTH
            }            
        }
    } 
    return $MAX_WIDTH   
}
function banner
{
    param (
        [string]$text,
        [bool]$flag = $false
    )
    if ($flag -eq $true) {
        Clear-Host
    }
    $LEN = textWidth($text)
    for ($n = 0; $n -le $LEN; $n++) {
        Write-Host -NoNewline "-"
    }
    Write-Host "`n$text"
    for ($n = 0; $n -le $LEN; $n++) {
        Write-Host -NoNewline "-"
    }
    Write-Host "`n`n"
}
function evalOutput( $expected )
{
    Write-Host "`n`n"
    banner("Expected Output:`n$expected")
    Read-Host "Press return to proceed"
    Write-Host "`n`n"
}

