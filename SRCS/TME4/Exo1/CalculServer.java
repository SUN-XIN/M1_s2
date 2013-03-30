package TME4.Exo1;

import TME4.Exo1.CalculPackage.NotZero;

public class CalculServer extends CalculPOA {

	@Override
	public double add(double x, double y) {
		return x+y;
	}

	@Override
	public double sub(double x, double y) {
		return x-y;
	}

	@Override
	public double mult(double x, double y) {
		return x*y;
	}

	@Override
	public double div(double x, double y) throws NotZero {
		if(y == 0){
			throw new NotZero("division par 0");
		}
		return x/y;
	}

}
