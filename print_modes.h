#ifndef PRINT_MODES_H_INCLUDED
#define PRINT_MODES_H_INCLUDED

namespace print_modes {
    enum PrintModes{SIMPLE, VISUAL, LATEX};
    PrintModes printMode = SIMPLE;
    size_t MAX_WIDTH = 9;
}

#endif // PRINT_MODES_H_INCLUDED
