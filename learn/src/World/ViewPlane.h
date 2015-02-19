#pragma once

class ViewPlane {

private:
	int hres, vres;
    int numSamples;

public:
    ViewPlane(const int hres, const int vres, const int numSamples) :
        hres(hres), vres(vres), numSamples(numSamples) {
	}

	int getHRes() const {
		return hres;
	}
    int getVRes() const {
        return vres;
    }
    int getNumSamples() const {
        return numSamples;
    }
};