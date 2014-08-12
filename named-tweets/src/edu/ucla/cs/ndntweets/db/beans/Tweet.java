package edu.ucla.cs.ndntweets.db.beans;

public class Tweet {

	private Integer id = null;
	private Integer userId = null;
	private String text = null;
	
	public Tweet(String text, Integer userId) {
		setText(text);
		setUserId(userId);
	}
	
	public Tweet(Integer id, String text, Integer userId) {
		setId(id);
		setText(text);
		setUserId(userId);
	}
	
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public Integer getUserId() {
		return userId;
	}
	public void setUserId(Integer userId) {
		this.userId = userId;
	}
	public String getText() {
		return text;
	}
	public void setText(String text) {
		this.text = text;
	}

}
