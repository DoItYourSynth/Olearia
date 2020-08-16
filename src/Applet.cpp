#include "Applet.hpp"

// AMP
// ----------------------
Amp::Amp(float sample_rate) {
	amp_l = 0.0f;
	amp_r = 0.0f;
}

void Amp::Control(float in_l, float in_r) {
	amp_l = in_l;
	amp_r = in_r;
}

float* Amp::Process(float in_l, float in_r) {
	static float out[2];
	out[0] = in_l * amp_l;
	out[1] = in_r * amp_r;
	return out;
}

void Amp::Draw(int *out, int width, int height) {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			out [y * width + x] = 0;
			if (x > 0 && x < width / 2 && y > (height - amp_l * height)) {
				out[y * width + x] = 1;
			}

			if (x > width / 2 && y > (height - amp_r * height)) {
				out[y * width + x] = 1;
			}
		}
	}
}

// NOISE
// ----------------------
Noise::Noise(float sample_rate) {
	amp_ = 1.0;
}

void Noise::Control(float in_l, float in_r) {
	amp_ = in_l;
}

float* Noise::Process(float in_l, float in_r) {
	static float out[2];
	float v = (rand() / (RAND_MAX + 1.)) * amp_;
	out[0] = v;
	out[1] = v;
	return out;
}

void Noise::Draw(int *out, int width, int height) {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			out [y * width + x] = 0;
		}
	}
}

// OSC
// ----------------------
Osc::Osc(float sample_rate) {
	car_freq_ = 440.0f;
	mod_freq_ = 110.0f;
	mod_index_ = 100.0;
	osc_one = new Oscillator(sample_rate);
	osc_two = new Oscillator(sample_rate);
}

Osc::~Osc() {
	delete osc_one;
	delete osc_two;
}

float v_to_freq(float v) {
	return (16.352 * 4) * powf(2.0, abs(v) * 5);
}

void Osc::Control(float in_l, float in_r) {
	car_freq_ = v_to_freq(in_l);
	//mod_freq_ = v_to_freq(in_r);
	mod_index_ = in_r * 5000.0f;
}

float* Osc::Process(float in_l, float in_r) {
	static float out[2];
	// FM
	osc_one->SetFreq(car_freq_);
	float mod_f = osc_one->Process();
	osc_two->SetFreq(mod_f * mod_index_ + car_freq_);
	out[0] = mod_f;
	out[1] = osc_two->Process();
	return out;
}

void Osc::Draw(int *out, int width, int height) {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			out [y * width + x] = 0;
		}
	}
}
