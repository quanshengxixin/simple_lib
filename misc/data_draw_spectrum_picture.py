import librosa.display
import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf
import os
import tkinter
from tkinter.filedialog import askdirectory

def data_shape_get_channels(shape):
    if len(shape) == 1:
        return 1
    elif len(shape) == 2:
        return shape[0]
    else:
        return 0

def data_draw_spectrum_picture(file_path, out_dir):
    print(file_path)
    print(out_dir)
    file_name = os.path.split(file_path)[-1]
    print(file_name)
    if not os.path.isdir(out_dir):
        os.makedirs(out_dir)
    # y, sr = sf.read("test.raw", channels=1, samplerate=44100, subtype="PCM_16")
    music_data, sr = librosa.load(file_path, mono=False)
    channels = data_shape_get_channels(music_data.shape)
    for channel in range(channels):
        y = music_data[channel]
        out_file_name = os.path.splitext(file_name)[0] + "_" + str(channel) + "ch.png"
        out_file_path = os.path.join(out_dir, out_file_name)
        print(out_file_name)
        print(out_file_path)
        plt.figure(figsize=(10, 4))

        D = librosa.amplitude_to_db(np.abs(librosa.stft(y)), ref=np.max)

        librosa.display.specshow(D, y_axis='log', x_axis="s", sr=sr)
        plt.colorbar(format='%+2.0f dB')
        plt.title(u'Log-frequency power spectrum/'+out_file_name)
        plt.tight_layout()
        plt.savefig(out_file_path, dpi=500)
        plt.cla()

if __name__ == "__main__":
    file_name = "16K_4CH_16bit.wav"
    file_path = os.path.abspath(os.path.join(".", file_name))

    out_dir = os.path.abspath(os.path.join(".", "out_picture"))
    print(out_dir)
    # data_draw_spectrum_picture(file_path, out_dir)
