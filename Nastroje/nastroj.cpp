#include "nastroj.h"

using namespace Nastroje;

Nastroj::Nastroj(Dokumenty::Dokument *dokument, QObject *parent)
    : QObject(parent), _dokument(dokument) {}
