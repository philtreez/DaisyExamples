#include "daisy_patch.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisyPatch patch;
Oscillator osc;
bool isSineWave = true;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        // Lesen Sie den Potentiometerwert für die Tonhöhe (Pitch)
        float pitchPot = patch.controls[patch.CTRL_1].Process();

        // Lesen Sie den Potentiometerwert für die Wellenformauswahl
        float waveformPot = patch.controls[patch.CTRL_3].Process();  // CV_3 für Wellenform

        // Stellen Sie die Wellenform basierend auf dem Potentiometer ein
        if (waveformPot < 0.5f)
        {
            osc.SetWaveform(osc.WAVE_SIN);
            isSineWave = true;
        }
        else
        {
            osc.SetWaveform(osc.WAVE_TRI);
            isSineWave = false;
        }

        // Setzen Sie die Tonhöhe (Pitch) basierend auf dem Potentiometerwert
        float pitch = 40.0f + pitchPot * 48.0f;  // Bereich: C2 bis C6

        // Berechnen Sie den nächsten Ausgang des Oszillators
        float output = osc.Process();

        // Passen Sie die Tonhöhe des Oszillators an
        osc.SetFreq(mtof(pitch));

        // Senden Sie den Oszillator-Ausgang zum Ausgang "OUT_L[i]"
        out[0][i] = output;
        out[1][i] = output;  // Für Stereo-Ausgabe, OUT_R[i] verwenden
    }
}

int main(void)
{
    // Initialisieren Sie das Daisy Patch und den Oszillator
    patch.Init();
    osc.Init(patch.AudioSampleRate());
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetAmp(0.5f);

    while (1)
    {
        // Hauptprogrammschleife
        patch.ProcessAnalogControls();
    }
}
