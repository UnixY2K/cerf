from __future__ import annotations

import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Callable, Optional

import install_registry
import install_shortcuts

LogFn = Callable[[str], None]

USER_DATA = ("cerf.json", "devices", "screenshots", "CF.IMG", "cerf.log",
             "cerf.crash.log")

SELF_DELETE_ATTEMPTS = 60


def _keep(name: str) -> bool:
    return name.lower() in set(n.lower() for n in USER_DATA)


def _remove(path: Path, log: LogFn) -> None:
    try:
        if path.is_dir() and not path.is_symlink():
            shutil.rmtree(path)
        else:
            path.unlink()
    except OSError as exc:
        log("  {} not removed ({})".format(path.name, exc))


def remove_installation(install_dir: Path, delete_user_data: bool,
                        log: LogFn) -> Optional[Path]:
    own = Path(sys.executable).resolve() if getattr(sys, "frozen", False) \
        else None
    locked: Optional[Path] = None
    try:
        entries = sorted(install_dir.iterdir())
    except OSError as exc:
        log("  {} cannot be read ({})".format(install_dir, exc))
        return None
    for entry in entries:
        if not delete_user_data and _keep(entry.name):
            continue
        try:
            is_own = own is not None and entry.resolve() == own
        except OSError:
            is_own = False
        if is_own:
            locked = entry
            continue
        log("  {}".format(entry.name))
        _remove(entry, log)
    return locked


def remove_shell_integration(log: LogFn) -> None:
    log("Removing the Start menu entries and the desktop shortcut")
    install_shortcuts.remove_start_menu_entries()
    install_shortcuts.remove_desktop_shortcut()
    log("Removing the Programs and Features entry")
    install_registry.unregister()


def _self_delete_script() -> str:
    return "\r\n".join([
        "@echo off",
        "for /L %%N in (1,1,{}) do (".format(SELF_DELETE_ATTEMPTS),
        '  if not exist "%~1" goto done',
        '  del /F /Q "%~1" >nul 2>&1',
        "  ping -n 2 127.0.0.1 >nul",
        ")",
        ":done",
        'if not "%~2"=="" rmdir "%~2" >nul 2>&1',
        'del /F /Q "%~f0" >nul 2>&1',
    ]) + "\r\n"


def _self_delete_command(helper: Path, locked: Path, install_dir: Path,
                         remove_directory: bool) -> str:
    quoted = " ".join('"{}"'.format(p) for p in (
        helper, locked, install_dir if remove_directory else ""))
    return 'cmd.exe /s /c "{}"'.format(quoted)


def schedule_self_delete(locked: Optional[Path], install_dir: Path,
                         remove_directory: bool) -> None:
    if locked is None:
        if remove_directory:
            try:
                install_dir.rmdir()
            except OSError:
                pass
        return

    helper = Path(tempfile.gettempdir()) / "cerf-uninstall.cmd"
    try:
        helper.write_text(_self_delete_script(), encoding="ascii")
        subprocess.Popen(_self_delete_command(helper, locked, install_dir,
                                              remove_directory),
                         cwd=str(helper.parent),
                         creationflags=getattr(subprocess, "DETACHED_PROCESS", 0)
                         | getattr(subprocess, "CREATE_NO_WINDOW", 0))
    except OSError:
        pass
