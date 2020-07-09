# Sequencer

This project supports the creation and execution of automation scripts.
It comprises three parts:
* A library for executing and (de)serializing automation scripts.
* A Qt GUI that allows a user to easily create automation scripts and that injects user interactions into the library.
* A CLI application that can execute serialized automation scripts and injects CLI user interactions.

## Features

* The Model is fully decoupled from any Qt machinery.
* The ViewModel is based on QAbstractItemModel and exposes a model to a standard QTreeView.

## Issues

* There is an issue with the fonts in Qt (when used from local source build). This can be temporarily fixed by running the application as follows:

```bash
QT_QPA_FONTDIR=/usr/share/fonts/dejavu ./Sequencer
```
