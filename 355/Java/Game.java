// EDITED BY: Anthony Arrott 10853267

import java.io.*;

class Game {
    private static final double noBurn = 0.0;

    private static final double fullBurn = 1.0;

    private static final double deltaTime = 0.5;

    private static void play(Rocket rocket) throws RocketException, IOException{
    	BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    	String cmp;
        while (true) {
            /* read input and decide whether to burn or not */
        	cmp = br.readLine();

        	if (cmp.equals("b")){
        		rocket.move(deltaTime, fullBurn); // move .5 seconds at 1.0
        		System.out.println(rocket.getHeightString());
        	}
           	else if (cmp.equals("")){
        		rocket.move(deltaTime, noBurn); // move .5 seconds at 0.0
        		System.out.println(rocket.getHeightString());
        	}
           	else{
           		// give user helpful message to run program correctly
           		System.out.println("Use 'b' then enter to move at full burn or just enter to use no burn\n");
           	}
            

        	
        }

    }

    public static void main(String[] args) throws RocketException, IOException{
        /* create a planet with gravity 0.3 and surface at 0.0 */
        Planet pluto = new Planet(0.3, 0.0, "pluto");
        /* create a rocket with engine strength 2.0, 20.0 units of fuel */
        Rocket rocket = new Rocket(2.0, 20.0, pluto);
        rocket.setHeight(50.0);
        try {
            play(rocket);
        } catch (RocketException v) {
            System.out.println(v.getMessage());
        }
    }

} // end of Game class
