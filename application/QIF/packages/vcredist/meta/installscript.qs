function Component() {
}

function extractFileName(path) {
    const fullName = path.substring(path.lastIndexOf('/') + 1);

    const index = fullName.lastIndexOf('.');
    if (index >= 0) {
        return fullName.substring(0, index)
    }

    return fullName;
}

function install_msvc_runtime() {
	if (installer.value("os") === "win") {
		// Exit code 1638 produced by vcredist when there is already C++ runtime installed, so we probably can ignore it
		// see also https://amin-ahmadi.com/2019/06/29/qt-installer-framework-how-to-run-an-executable-during-installation/
		// 1602: Happens when the installation of Redistributable Package is cancelled.
		if (component.addElevatedOperation("Execute", "{0,1638}", "@TargetDir@\\vcredist_msvc2019_x86.exe"))
		{
			//QMessageBox.information("Installer", "install_msvc_runtime", "Execute ok", QMessageBox.Ok);
		}
		else
		{
			//QMessageBox.information("Installer", "install_msvc_runtime", "Execute failed", QMessageBox.Ok);
		}
	}
}

Component.prototype.createOperations = function() {
    // call default implementation to actually install README.txt!
    component.createOperations();

    install_msvc_runtime();
}
