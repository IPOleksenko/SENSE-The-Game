#pragma once

#include <string>

#define SECTION_LEN 500
enum class CheckPoint : int {
    IDLE        = 0,
    BEGIN       = 1,

    A_START = 1000,
    A_STOP = A_START + SECTION_LEN,

    B_START = 2000,
    B_STOP = B_START + SECTION_LEN,

    C_START = 3000,
    C_STOP = C_START + SECTION_LEN,

    D_START = 4000,
    D_STOP = D_START + SECTION_LEN,

    E_START = 5000,
    E_STOP = E_START + SECTION_LEN,

    F_START = 6000,
    F_STOP = F_START + SECTION_LEN,

    G_START = 7000,
    G_STOP = G_START + SECTION_LEN,

    H_START = 8000,
    H_STOP = H_START + SECTION_LEN,

    I_START = 9000,
    I_STOP = I_START + SECTION_LEN,

    J_START = 10000,
    J_STOP = J_START + SECTION_LEN,

    K_START = 11000,
    K_STOP = K_START + SECTION_LEN,

    L_START = 12000,
    L_STOP = L_START + SECTION_LEN,

    M_START = 13000,
    M_STOP = M_START + SECTION_LEN,

    N_START = 14000,
    N_STOP = N_START + SECTION_LEN,

    O_START = 15000,
    O_STOP = O_START + SECTION_LEN,

    P_START = 16000,
    P_STOP = P_START + SECTION_LEN,

    Q_START = 17000,
    Q_STOP = Q_START + SECTION_LEN,

    R_START = 18000,
    R_STOP = R_START + SECTION_LEN,

    S_START = 19000,
    S_STOP = S_START + SECTION_LEN,

    T_START = 20000,
    T_STOP = T_START + SECTION_LEN,

    FINAL_START = 25000,
    FINAL_STOP = FINAL_START + SECTION_LEN,
};

std::string getCheckpointText(const CheckPoint& checkPoint);

