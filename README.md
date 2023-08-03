# NJR4262
NJR4262(24GHz ドップラーレーダ)から呼吸波形を求めるプログラムを開発する。
Arduinoマイコンを使用して開発を行う。

## Software
| name | description |
| ---- | ---- |
| [VisualStudioCode(vscode)](https://azure.microsoft.com/ja-jp/products/visual-studio-code) | テキストエディタ |
| [platformio](https://platformio.org/) | vscodeの拡張機能</br>ArduinoマイコンのIDEとして機能する |
| [ArduinoSTL](https://registry.platformio.org/libraries/mike-matera/ArduinoSTL) | Arduinoでイテレータを使用するためのライブラリ |

## Hardware
| name | description |
| ---- | ---- |
| [ESP32-DevkitC](https://akizukidenshi.com/catalog/g/gM-11819/) | Arduinoマイコンボード |
| NJR4262 | 24GHz ドップラーレーダ |
| 増幅回路 | NJR4262のI/Q信号を増幅する回路 |
