package edu.ucla.cs.ndntweets.exceptions;

public class BadTweetException extends Exception {

	private static final long serialVersionUID = 9072311880435911852L;

	public BadTweetException(String message) {
		super(message);
	}

}
