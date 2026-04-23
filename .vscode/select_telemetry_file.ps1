# PowerShell script to open a file dialog and plot selected telemetry file
param(
    [string]$WorkspaceFolder
)

Add-Type -AssemblyName System.Windows.Forms

$fileDialog = New-Object System.Windows.Forms.OpenFileDialog
$fileDialog.InitialDirectory = $WorkspaceFolder
$fileDialog.Filter = "All Files (*.*)|*.*|JSON Files (*.json)|*.json"
$fileDialog.Title = "Select Telemetry File to Plot"

$result = $fileDialog.ShowDialog()

if ($result -eq [System.Windows.Forms.DialogResult]::OK) {
    $selectedFile = $fileDialog.FileName
    Write-Host "Selected file: $selectedFile"
    
    # Execute the Python plotting script
    $pythonScript = Join-Path $WorkspaceFolder "software\yeet\tools\plot_telemetry.py"
    python.exe $pythonScript $selectedFile --open
}
else {
    Write-Host "File selection cancelled"
    exit 1
}
