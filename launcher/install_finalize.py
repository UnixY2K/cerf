from __future__ import annotations

from pathlib import Path
from typing import Callable

import install_registry
import install_shortcuts
from app_paths import resolve_version, resolve_version_tuple
from install_options import InstallOptions

LogFn = Callable[[str], None]


def _plain_version() -> str:
    numbers = resolve_version_tuple()
    if numbers is None:
        return resolve_version()
    return ".".join(str(n) for n in numbers)


def finalize(install_dir: Path, options: InstallOptions, log: LogFn) -> None:
    if options.desktop_icon:
        log("Creating the desktop shortcut")
        try:
            install_shortcuts.create_desktop_shortcut(install_dir)
        except OSError as exc:
            log("  desktop shortcut not created ({})".format(exc))

    if options.start_menu:
        log("Creating the Start menu entries")
        try:
            install_shortcuts.create_start_menu_entries(install_dir)
        except OSError as exc:
            log("  Start menu entries not created ({})".format(exc))

    if not options.fresh:
        return

    log("Registering in Programs and Features")
    try:
        install_registry.register(install_dir, _plain_version())
    except OSError as exc:
        log("  not registered ({})".format(exc))
