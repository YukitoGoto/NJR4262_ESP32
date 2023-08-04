# NJR4262_ESP32
NJR4262(24GHz ドップラーレーダ)から呼吸波形を求めるプログラムを、ESP32にて開発する。

## 実行環境
### Software
| name | description |
| ---- | ---- |
| [VisualStudioCode(vscode)](https://azure.microsoft.com/ja-jp/products/visual-studio-code) | テキストエディタ |
| [platformio](https://platformio.org/) | vscodeの拡張機能</br>ArduinoマイコンのIDEとして機能する |
| [TelePlot](https://marketplace.visualstudio.com/items?itemName=alexnesnes.teleplot) | vscodeの拡張機能</br>シリアルプロッタとして機能する |
| [ArduinoSTL](https://registry.platformio.org/libraries/mike-matera/ArduinoSTL) | Arduinoでイテレータを使用するためのライブラリ |
### Hardware
| name | description |
| ---- | ---- |
| [ESP32-DevkitC](https://akizukidenshi.com/catalog/g/gM-11819/) | Arduinoマイコンボード |
| NJR4262 | 24GHz ドップラーレーダ |
| 増幅回路 | NJR4262のI/Q信号を増幅する回路 |

## ファイル
### lib
NJR4262から呼吸波形を求めるためのライブラリを格納している。
- `NJR4262_ESP32.h`
- `NJR4262_ESP32.cpp`
### src
メインコードを格納してる。`NJR4262_ESP32.h`を用いて呼吸波形をシリアル出力している。出力値はTeleplotを用いてグラフ化される。
- `main.cpp`
### hard
システムの画像と増幅回路の回路図を格納している。
- `RadarAmplifier.png`
![RadarAmplifier](https://github.com/YukitoGoto/NJR4262_ESP32/assets/63275740/d36242ff-1ec1-4412-80dc-4e34efa8d077)
- `System.jpg`
![System](https://github.com/YukitoGoto/NJR4262_ESP32/assets/63275740/988df86d-8222-4a38-87e2-3ab3d06a01cc)

増幅回路とESP32の接続は、デフォルトでは次のように対応している。
| 増幅回路(シンボル) | ESP32 pin |
| ---- | ---- |
| Vss | 5V |
| GND | GND |
| I Output(yellow) | A10(4) |
| Q Output(white) | A12(2) |
