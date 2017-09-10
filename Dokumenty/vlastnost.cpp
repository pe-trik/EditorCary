#include "vlastnost.h"

using namespace Dokumenty;

Vlastnost::Vlastnost(QString nazov, QObject *parent)
    : QObject(parent), _nazov(nazov) {}
