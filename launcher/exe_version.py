import re

from branding import AUTHOR, PRODUCT_NAME, copyright_line


def _defines(header_path):
    try:
        with open(header_path, "r", encoding="utf-8", errors="ignore") as f:
            text = f.read()
    except OSError:
        return {}
    out = {}
    for name in ("MAJOR", "MINOR", "PATCH", "BUILD_WORD"):
        match = re.search(r"#define\s+CERF_VERSION_" + name + r"\s+(\d+)", text)
        out[name] = int(match.group(1)) if match else 0
    return out


def version_tuple(header_path):
    d = _defines(header_path)
    if not d:
        return (0, 0, 0, 0)
    return (d["MAJOR"], d["MINOR"], d["PATCH"], d["BUILD_WORD"])


def version_string(header_path):
    return ".".join(str(n) for n in version_tuple(header_path))


def build(header_path, original_filename, internal_name, description):
    from PyInstaller.utils.win32 import versioninfo as vi

    numbers = version_tuple(header_path)
    text = version_string(header_path)
    strings = [
        vi.StringStruct("CompanyName", AUTHOR),
        vi.StringStruct("FileDescription", description),
        vi.StringStruct("FileVersion", text),
        vi.StringStruct("InternalName", internal_name),
        vi.StringStruct("LegalCopyright", copyright_line()),
        vi.StringStruct("OriginalFilename", original_filename),
        vi.StringStruct("ProductName", PRODUCT_NAME),
        vi.StringStruct("ProductVersion", text),
    ]
    return vi.VSVersionInfo(
        ffi=vi.FixedFileInfo(filevers=numbers, prodvers=numbers),
        kids=[vi.StringFileInfo([vi.StringTable("040904E4", strings)]),
              vi.VarFileInfo([vi.VarStruct("Translation", [0x0409, 0x04E4])])])
