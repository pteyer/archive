package edu.ucla.cs.ndntweets.twitter;

import java.util.LinkedList;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import edu.ucla.cs.ndntweets.db.beans.Tweet;
import edu.ucla.cs.ndntweets.db.beans.User;
import edu.ucla.cs.ndntweets.exceptions.BadTweetException;

public class UserHandler {

	private static final Log log = LogFactory.getLog(UserHandler.class);
	private static final int MAX_TEXT_LENGTH = 140;
	
	private User user = null;
	private List<Tweet> tweets = null;

	protected UserHandler() {}
	
	public List<Tweet> getTweets() {
		return tweets;
	}

	public void setTweets(List<Tweet> tweets) {
		this.tweets = tweets;
	}
	
	public void tweet(String message)
	throws BadTweetException {
		this.tweet(new Tweet(message, 0));
	}
	
	public void tweet(Tweet tweet) {
//		log.debug("User '" + user.getUsername() + "' tweets: " + tweet.getText());
		
		if (tweets == null) {
			tweets = new LinkedList<Tweet>();
		}
		tweets.add(tweet);
	}
	
//	public void setText(String text) 
//	throws BadTweetException {
//		
//		if (text.length() > MAX_TEXT_LENGTH) {
//			
//			String errmsg = "Tweet text too long (" + text.length()
//				+ "). Maximum length allowed is " + MAX_TEXT_LENGTH
//				+ " characters";
//
//			throw new BadTweetException(errmsg);
//			
//		} else if (text == null || text.length() == 0 || text.equals("")) {
//			
//			String errmsg = "Invalid tweet text";
//
//			throw new BadTweetException(errmsg);
//			
//		} else {
//			this.text = text;
//			log.debug("Tweet text set to: " + getText());
//		}
//	}
	
}
