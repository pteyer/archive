package edu.ucla.cs.ndntweets.twitter;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class UserFactory {

	private static final Log log = LogFactory.getLog(UserFactory.class);
	
	public static UserHandler newUser(String username, String password) {
		
		// TODO use the given username and password to create the new user
		// return the valid user if successful
		// throw exception if otherwise
		
		return new UserHandler();
	}
	
	public static UserHandler loadUser(String username, String password) {
		
		// TODO use the given username and password to authenticate the user
		// return the valid user
		// throw exception if user was invalid
		
		return null;
	}
	
}
