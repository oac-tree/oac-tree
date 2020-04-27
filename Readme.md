# Sequencer

This project is meant to support the easy creation of automation scripts.

## Features

* The Model is fully decoupled from any Qt machinery.
* The ViewModel is based on QAbstractItemModel and exposes a model to a standard QTreeView.

## Issues

* There is an issue with the fonts in Qt (when used from local source build). This can be temporarily fixed by running the application as follows:

```bash
QT_QPA_FONTDIR=/usr/share/fonts/dejavu ./Sequencer
```