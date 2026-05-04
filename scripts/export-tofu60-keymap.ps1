param(
    [string]$OutputDir = "",
    [string]$Keyboard = "dztech/tofu60",
    [string]$Keymap = "lucidust",
    [string]$Layout = "LAYOUT_60_ansi_split_bs_rshift",
    [string[]]$LayerNames = @("Base", "Nav", "Sym", "Fn", "Num", "Boot"),
    [int]$Columns = 0
)

$ErrorActionPreference = "Stop"

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
if ([string]::IsNullOrWhiteSpace($OutputDir)) {
    $OutputDir = Join-Path $RepoRoot "build\keymap-drawer"
}
$OutputDir = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($OutputDir)

$ToolsVenv = Join-Path $env:TEMP "qmk-keymap-tools"
$QmkHome = Join-Path $env:TEMP "qmk_firmware_c2json"
$KeymapC = Join-Path $RepoRoot "keyboards\dztech\tofu60\keymaps\lucidust\keymap.c"
$JsonOut = Join-Path $OutputDir "tofu60-keymap.json"
$YamlOut = Join-Path $OutputDir "tofu60-keymap.yaml"
$CombosOut = Join-Path $OutputDir "tofu60-combos.yaml"
$SvgOut = Join-Path $OutputDir "tofu60-keymap.svg"

function Invoke-Checked {
    param([string]$FilePath, [string[]]$Arguments)

    & $FilePath @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "Command failed: $FilePath $($Arguments -join ' ')"
    }
}

function Convert-KeycodeToLegend {
    param([string]$Keycode)

    $trimmed = $Keycode.Trim()
    $legendOverrides = @{
        "KC_ESC"  = "ESC"
        "KC_TAB"  = "TAB"
        "KC_BSPC" = "BSPC"
        "KC_DEL"  = "DEL"
        "KC_ENT"  = "ENT"
        "KC_SPC"  = "SPC"
        "KC_CAPS" = "CAPS"
        "KC_LBRC" = "["
        "KC_RBRC" = "]"
        "KC_LPRN" = "("
        "KC_RPRN" = ")"
        "KC_LT"   = "<"
        "KC_GT"   = ">"
        "KC_RALT" = "RALT"
    }

    if ($legendOverrides.ContainsKey($trimmed)) {
        return $legendOverrides[$trimmed]
    }

    if ($trimmed -match "^C\((KC_)?(?<key>[^)]+)\)$") {
        return "Ctrl+$($Matches.key)"
    }

    if ($trimmed.StartsWith("KC_")) {
        return $trimmed.Substring(3)
    }

    return $trimmed
}

function Write-ComboYaml {
    param(
        [string]$KeymapPath,
        [string]$JsonPath,
        [string]$OutputPath
    )

    $source = Get-Content -Raw $KeymapPath
    $source = [regex]::Replace($source, "(?s)/\*.*?\*/", "")
    $source = [regex]::Replace($source, "(?m)//.*$", "")
    $keymapJson = Get-Content -Raw $JsonPath | ConvertFrom-Json
    $baseLayer = @($keymapJson.layers[0])

    $comboArrays = @{}
    $arrayRegex = [regex]"const\s+uint16_t\s+PROGMEM\s+(?<name>\w+)\[\]\s*=\s*\{(?<keys>[^}]+)\};"
    foreach ($match in $arrayRegex.Matches($source)) {
        $keys = @(
            $match.Groups["keys"].Value.Split(",") |
                ForEach-Object { $_.Trim() } |
                Where-Object { $_ -and $_ -ne "COMBO_END" }
        )
        $comboArrays[$match.Groups["name"].Value] = $keys
    }

    $legendByComboId = @{
        "JK_LPRN"      = "("
        "KL_RPRN"      = ")"
        "XC_COPY"      = "Copy"
        "CV_PASTE"     = "Paste"
        "XV_CUT"       = "Cut"
    }

    $lines = [System.Collections.Generic.List[string]]::new()
    $lines.Add("combos:")

    $comboRegex = [regex]"\[(?<id>\w+)\]\s*=\s*COMBO\((?<array>\w+),\s*(?<binding>.+?)\),"
    foreach ($match in $comboRegex.Matches($source)) {
        $comboId = $match.Groups["id"].Value
        $arrayName = $match.Groups["array"].Value
        if (!$comboArrays.ContainsKey($arrayName)) {
            continue
        }

        $positions = @()
        foreach ($key in $comboArrays[$arrayName]) {
            $index = [array]::IndexOf($baseLayer, $key)
            if ($index -lt 0) {
                throw "Cannot find combo trigger key '$key' for $comboId in base layer."
            }
            $positions += $index
        }

        if ($positions.Count -lt 2) {
            continue
        }

        $legend = if ($legendByComboId.ContainsKey($comboId)) {
            $legendByComboId[$comboId]
        } else {
            Convert-KeycodeToLegend $match.Groups["binding"].Value
        }

        $positionText = $positions -join ", "
        $safeLegend = $legend.Replace("'", "''")
        $lines.Add("  - p: [$positionText]")
        $lines.Add("    k: '$safeLegend'")
        $lines.Add("    l: [Base]")
        $lines.Add("    a: top")
    }

    if ($lines.Count -eq 1) {
        $lines.Add("[]")
    }

    Set-Content -Path $OutputPath -Value ($lines -join [Environment]::NewLine) -Encoding UTF8
}

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

if (!(Test-Path $ToolsVenv)) {
    Invoke-Checked "python" @("-m", "venv", $ToolsVenv)
}

$VenvPython = Join-Path $ToolsVenv "Scripts\python.exe"
$KeymapDrawer = Join-Path $ToolsVenv "Scripts\keymap.exe"
Invoke-Checked $VenvPython @("-m", "pip", "install", "--disable-pip-version-check", "--quiet", "--upgrade", "pip")
Invoke-Checked $VenvPython @("-m", "pip", "install", "--disable-pip-version-check", "--quiet", "qmk", "keymap-drawer")

if (!(Test-Path $QmkHome)) {
    Invoke-Checked "git" @("clone", "--filter=blob:none", "--sparse", "https://github.com/qmk/qmk_firmware.git", $QmkHome)
    Invoke-Checked "git" @("-C", $QmkHome, "sparse-checkout", "set", "lib", "data", "quantum", "platforms", "drivers", "layouts/default", "layouts/community", "keyboards/dztech/tofu60")
} else {
    Invoke-Checked "git" @("-C", $QmkHome, "sparse-checkout", "add", "lib", "data", "quantum", "platforms", "drivers", "layouts/default", "layouts/community", "keyboards/dztech/tofu60")
}

$QmkPython = ""
try {
    $QmkPython = (& uv python find 3.13).Trim()
} catch {
    $QmkPython = ""
}

if ([string]::IsNullOrWhiteSpace($QmkPython) -or !(Test-Path $QmkPython)) {
    $QmkPython = (Get-Command python).Source
}

$OldPythonPath = $env:PYTHONPATH
$OldQmkHome = $env:QMK_HOME
$OldQmkUserspace = $env:QMK_USERSPACE

try {
    $env:PYTHONPATH = Join-Path $ToolsVenv "Lib\site-packages"
    $env:QMK_HOME = $QmkHome
    $env:QMK_USERSPACE = $RepoRoot

    Invoke-Checked $QmkPython @(
        "-c",
        "import qmk_cli.script_qmk as s; raise SystemExit(s.main())",
        "c2json",
        "--no-cpp",
        "-kb", $Keyboard,
        "-km", $Keymap,
        "-o", $JsonOut,
        $KeymapC
    )
} finally {
    $env:PYTHONPATH = $OldPythonPath
    $env:QMK_HOME = $OldQmkHome
    $env:QMK_USERSPACE = $OldQmkUserspace
}

$ParseArgs = @("parse", "-q", $JsonOut, "-l") + $LayerNames
if ($Columns -gt 0) {
    $ParseArgs += @("-c", "$Columns")
}
$ParseArgs += @("-o", $YamlOut)
Invoke-Checked $KeymapDrawer $ParseArgs
Write-ComboYaml -KeymapPath $KeymapC -JsonPath $JsonOut -OutputPath $CombosOut
$KeyboardJson = Join-Path $QmkHome "keyboards\dztech\tofu60\keyboard.json"
Invoke-Checked $KeymapDrawer @("draw", "-j", $KeyboardJson, "-l", $Layout, $YamlOut, $CombosOut, "-o", $SvgOut)

Write-Output "JSON: $JsonOut"
Write-Output "YAML: $YamlOut"
Write-Output "Combos: $CombosOut"
Write-Output "SVG:  $SvgOut"
