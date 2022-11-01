#ifndef SAMPLECODEMODULE_EXCEPTIONTESTER_H
#define SAMPLECODEMODULE_EXCEPTIONTESTER_H

// Ambas excepciones reciben fd pero no los usan internamente, esto lo hacemos para que funciones correctamente su llamado en la shell
void divideZero(int argc, char ** argv);
void invalidOpCode(int argc, char ** argv);

#endif //SAMPLECODEMODULE_EXCEPTIONTESTER_H