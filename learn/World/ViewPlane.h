#pragma once

class ViewPlane {

private:
	int hres, vres;

public:
	ViewPlane(const int hres, const int vres) {
		this->hres = hres;
		this->vres = vres;
	}

	int getHRes() const {
		return hres;
	}
	int getVRes() const {
		return vres;
	}
};