// Copyright 2019-2020 the openage authors. See copying.md for legal info.

#include "presenter.h"

#include <eigen3/Eigen/Dense>

#include "log/log.h"
#include "renderer/gui/gui.h"
#include "renderer/gui/integration/public/gui_application_with_logger.h"
#include "renderer/gui/qml_info.h"
#include "renderer/resources/texture_info.h"
#include "renderer/window.h"
#include "util/path.h"

namespace openage::presenter {

Presenter::Presenter(const util::Path &root_dir) :
	root_dir{root_dir} {}


void Presenter::run() {
	log::log(INFO << "presenter launching...");

	this->gui_app = this->init_window_system();
	this->window = renderer::Window::create("openage presenter test", 800, 600);

	this->renderer = this->window->make_renderer();

	//// -- gui initialization
	util::Path qml_root = this->root_dir / "assets" / "qml";
	if (not qml_root.is_dir()) {
		throw Error{ERR << "could not find qml root folder " << qml_root};
	}

	util::Path qml_assets = this->root_dir / "assets";
	if (not qml_assets.is_dir()) {
		throw Error{ERR << "could not find asset root folder " << qml_assets};
	}

	util::Path qml_root_file = qml_root / "main.qml";
	if (not qml_root_file.is_file()) {
		throw Error{ERR << "could not find main.qml file " << qml_root_file};
	}

	// TODO: in order to support qml-mods, the fslike and filelike
	//       library has to be integrated into qt. For now,
	//       figure out the absolute paths here and pass them in.

	this->gui = std::make_shared<renderer::gui::GUI>(
		this->gui_app, // Qt application wrapper
		this->window, // window for the gui
		qml_root_file, // entry qml file, absolute path.
		qml_root, // directory to watch for qml file changes
		qml_assets, // qml data: Engine *, the data directory, ...
		this->renderer // openage renderer
	);

	auto gui_pass = this->gui->get_render_pass();

	while (not this->window->should_close()) {
		this->gui_app->process_events();
		// this->gui->process_events();

		this->gui->render();
		this->renderer->render(gui_pass);

		this->renderer->check_error();

		this->window->update();
	}
	log::log(MSG(info) << "draw loop exited");

	this->window->close();
}

std::shared_ptr<qtgui::GuiApplication> Presenter::init_window_system() {
	return std::make_shared<renderer::gui::GuiApplicationWithLogger>();
}

} // namespace openage::presenter
