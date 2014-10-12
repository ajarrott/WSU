// EDITED BY: Anthony Arrott 10853267

import java.text.DecimalFormat;

class Rocket {
    static final RocketException crashed = new RocketException("Crashed and Burned");
    static final RocketException landed = new RocketException("Safely Landed");
    private double velocity = 0.0; // default to 0
    private double height = 50.0; // default to 50.0
    private Planet planet;
    private static final double safeVelocity = -1.0; // Land within this
                                                       // velocity
    // initialize engineStr and fuel to 0;
    private double engineStrength = 0;
    private double fuel = 0;
    
    public void move(double dt, double burnRate) {
        /* note that dt is measured in seconds */
    	
        if (!reachedSurface()) {
            /* update the height, velocity and fuel */
        	nextHeight(dt);
        	nextVelocity(burnRate, dt);
        	nextFuel(burnRate, dt);
        }
    }
  
    private boolean reachedSurface() {
        /* true if rocket is at or below the planet's surface */
    	if ( height <= planet.getGround() ){
    		return true;
    	}
    	else{
    		return false;
    	}
    }
    
    private void nextFuel(double burnRate, double deltaTime){
    	// calculate fuel
    	fuel -= (burnRate * deltaTime);
    }
    
    private void nextHeight(double deltaTime) {
        height += (velocity * deltaTime);
        if (reachedSurface()) {
        	try{
        		if (velocity < safeVelocity) {
        			throw crashed;
        		} 
        		else {
        			/* throw landed exception */
        			throw landed;
        		}
        	}
        	catch (RocketException r) {
        		System.out.println(r.getMessage());
        		System.out.println(getState());
        		System.exit(1);
        	}
        }
    }
    
    private void nextVelocity(double burnRate, double deltaTime) {
        velocity += (engineStrength * burnRate) - (planet.getGravity())
                * deltaTime;
    }   
    
    public String getHeightString() {
        double maxHeight = (height > 60.0) ? height : 60.0;
        double belowGround = planet.getGround() - 10.0;
        int size = (int) (maxHeight - belowGround) + 1;
        char[] buffer = new char[size];
        DecimalFormat df = new DecimalFormat(" ###.##");

        int groundPosition = (int) (planet.getGround() - belowGround);

        for (int i = 0; i < size; i++)
            buffer[i] = ' ';
        int adjustedPosition = (int) (height - belowGround);
        adjustedPosition = (adjustedPosition <= 0) ? 0 : adjustedPosition;
        buffer[groundPosition] = '|';
        buffer[adjustedPosition] = '*';
        return (new String(buffer) + " " + df.format(velocity));
    }
    
    // setters
    public void setHeight(double h){
    	height = h;
    }
    
    // getters
    public double getHeight(){
    	return height;
    }
    
    public double getVelocity(){
    	return velocity;
    }
    
    public Planet getPlanet(){
    	return planet;
    }
    
    // return current state of the rocket
    public String getState(){
    	DecimalFormat df = new DecimalFormat(" ####.##");
    	
    	String s0 = "HEIGHT " + df.format(height) + " ";
    	String s1 = "VELOCITY " + df.format(velocity) + " ";
    	String s2 = "FUEL " + df.format(fuel);
    	
    	return s0 + s1 + s2;
    }
    
    // constructor
    public Rocket(double eStr, double f, Planet p){
    	engineStrength = eStr;
    	fuel = f;
    	planet = p;
    }
}
