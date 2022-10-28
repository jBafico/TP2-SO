#ifndef SAMPLECODEMODULE_EXCEPTIONTESTER_H
#define SAMPLECODEMODULE_EXCEPTIONTESTER_H

// Ambas excepciones reciben fd pero no los usan internamente, esto lo hacemos para que funciones correctamente su llamado en la shell
void divideZero(uint8_t fd);
void invalidOpCode(uint8_t fd);

#endif //SAMPLECODEMODULE_EXCEPTIONTESTER_H