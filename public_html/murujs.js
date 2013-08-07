/* Wrap stuff in your own class/object to avoid polluting
 * the global namespace. Then again, we must check if our
 * name of choice is in use.
 */
if (!MuruJS) {
// Create the empty object.
	var MuruJS = {};		
// The snarky comment is hidden by default.
	MuruJS.commentVisible = false;
// When this function is called, reverse the display style of 
// le snarky comment.
	MuruJS.bodyColor = "#AAA";
	MuruJS.displayComment = function () {
		item = document.getElementById('contact-comment');
		item.style.display = MuruJS.commentVisible ? 'none' : 'block';
		MuruJS.commentVisible = !MuruJS.commentVisible;
		return true;
	}

// Green. Green EVERYWHERE. 
// Didn't get the meme?
// See http://knowyourmeme.com/memes/x-x-everywhere - mildly NSFW
	MuruJS.greenText = function () {
		MuruJS.bodyColor = MuruJS.bodyColor == "#0F0" ? "#AAA" : "#0F0";
		document.body.style.color = MuruJS.bodyColor;
		return MuruJS.bodyColor;
	}

// Add the comment visibility controller to the various events as handler.
// Call this after the body, so that the DOM is loaded. document.load 
// doesn't work well. :( I'm doing something wrong.
	MuruJS.init = function () {
		document.getElementById("contact-comment").parentNode.parentNode.onmouseover = MuruJS.displayComment;
		document.getElementById("contact-comment").parentNode.parentNode.onmouseout = MuruJS.displayComment;
		document.getElementById("contact-comment").parentNode.parentNode.onclick = MuruJS.displayComment;
		return true;
	}
}

