// EDITED BY: Anthony Arrott 10853267

class Planet {

    private double gravity; /* units: (m/s/s) */
    private double ground;  /* units: m */
    private String name;

    // getters
    public double getGravity(){
    	return gravity;
    }
    
    public double getGround(){
    	return ground;
    }
    
    public String getName(){
    	return name;
    }
    
    // constructor
    public Planet(double g, double m, String p) {
    	gravity = g;
    	ground = m;
    	name = p;
    } // less boring planet
}
