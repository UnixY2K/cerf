from __future__ import annotations

from pathlib import Path
from typing import Callable, Optional

from bundles import BundleError
from update_channel_stable import fetch_latest_release
from upgrade_download import download_upgrade
from upgrade_process import UpgradeError

LogFn = Callable[[str], None]
ProgressFn = Callable[[str, int, Optional[int]], None]


def stage_release(install_dir: Path, log: LogFn, progress: ProgressFn) -> str:
    try:
        release = fetch_latest_release()
    except BundleError as exc:
        raise UpgradeError("cannot reach the CERF release feed: {}".format(exc))

    log("Latest release: CE Runtime Foundation {}".format(release.tag))
    try:
        install_dir.mkdir(parents=True, exist_ok=True)
    except OSError as exc:
        raise UpgradeError("cannot create {}: {}".format(install_dir, exc))

    download_upgrade(release, install_dir, log, progress)
    return release.tag
