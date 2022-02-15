#include "lib_graph.h"

c_matrix vectorProduct(c_matrix V1, c_matrix V2) {
	c_matrix temp(3, 1);
	temp(0, 0) = V1(1) * V2(2) - V1(2) * V2(1);
	temp(1, 0) = V1(2) * V2(0) - V1(0) * V2(2);
	temp(2, 0) = V1(0) * V2(1) - V1(1) * V2(0);
	return temp;
}

double scalarProduct(c_matrix V1, c_matrix V2) {
	return (V1(0) * V2(0) + V1(1) * V2(1) + V1(2) * V2(2));
}

double moduleVector(c_matrix V)
{
	return sqrt(V(0) * V(0) + V(1) * V(1) + V(2) * V(2));
}

double cosVectors(c_matrix V1, c_matrix V2) {
	return (scalarProduct(V1, V2) / (moduleVector(V1) * moduleVector(V2)));
}

c_matrix sphereToCart(c_matrix PView) {
	c_matrix R(3, 1);
	R(0, 0) = PView(0) * cos(PView(1)) * sin(PView(2));
	R(1, 0) = PView(0) * sin(PView(1)) * sin(PView(2));
	R(2, 0) = PView(0) * cos(PView(2));				
	return R;
}