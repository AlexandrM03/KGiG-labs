#pragma once
#include "c_matrix.h"

c_matrix vectorProduct(c_matrix V1, c_matrix V2);
double scalarProduct(c_matrix V1, c_matrix V2);
double moduleVector(c_matrix V);
double cosVectors(c_matrix V1, c_matrix V2);
c_matrix sphereToCart(c_matrix PView);