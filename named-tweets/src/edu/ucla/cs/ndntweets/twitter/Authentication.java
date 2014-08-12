package edu.ucla.cs.ndntweets.twitter;

import java.io.File;
import java.io.PrintStream;
import java.util.Scanner;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import twitter4j.Twitter;
import twitter4j.TwitterException;
import twitter4j.TwitterFactory;
import twitter4j.http.AccessToken;
import twitter4j.http.RequestToken;

public class Authentication {
	
	private static final Log log = LogFactory.getLog(Authentication.class);

	private static final String CONSUMER_KEY = "jzUxGmuclMiheKkttiUg";
	private static final String CONSUMER_SECRET = "";

	private Twitter twitter = null;

	public Twitter authorize(String userId) throws Exception {
		log.info("Authorizing user " + userId);
		
		TwitterFactory twf = new TwitterFactory();
		
		AccessToken at = loadAccessToken(userId);
		
		if (at != null) {
			log.debug("User has already retrieved an access token");
		} else {
			log.debug("User never retrieved an accesss token before");
			at = promptForAccessToken();
			storeAccessToken(userId, at);
		}

		twitter = twf.getOAuthAuthorizedInstance(
				CONSUMER_KEY, CONSUMER_SECRET, at);
		
		return twitter;
	}

	public AccessToken loadAccessToken(String userId) 
	throws Exception {
		AccessToken at = null;
		log.debug("Checking the access token repo for user " + userId);
		
		File ft = new File("/tmp/token");
		Scanner in = new Scanner(ft);
		while (in.hasNext()) {
			String line = in.nextLine();
			String t = null;
			String st = null;
			if (line.equals(userId)) {
				t = in.nextLine();
				st = in.nextLine();
				at = new AccessToken(t, st);
				break;
			}
		}
		
		return at;
	}
	
	public AccessToken promptForAccessToken() 
	throws Exception {
		AccessToken at = null;
		log.debug("Prompting user to retrieve access token");
		
		RequestToken rt = twitter.getOAuthRequestToken();
		System.out.println(
				"Open the following URL and grant access to your account: ");
		System.out.println(rt.getAuthorizationURL());
		System.out.print(
				"Enter the PIN (if aviailable) or just hit enter.[PIN]: ");
		Scanner sc = new Scanner(System.in);
		String pin = sc.nextLine();
		
		try {
			if (pin.length() > 0) {
				at = twitter.getOAuthAccessToken(rt, pin);
			} else {
				at = twitter.getOAuthAccessToken();
			}
		} catch (TwitterException e) {
			if (e.getStatusCode() == 401) {
				log.error("Unable to get the access token.");
			} else {
				log.error(e.getMessage());
			}
		}
		
		return at;
	}

	public void storeAccessToken(String userId, AccessToken at) 
	throws Exception {
		File ft = new File("/tmp/token");
		PrintStream out = new PrintStream(ft);
		out.println(userId);
		out.println(at.getToken());
		out.println(at.getTokenSecret());
		out.println("^^^");
	}
}
