// Create a function to add the translate popup
function addTranslatePopup() {
  // Create the popup container element
  alert("Create the popup container element")
  var popupContainer = document.createElement("div");
  popupContainer.id = "translatePopup";
  popupContainer.style.position = "fixed";
  popupContainer.style.top = "20px";
  popupContainer.style.right = "20px";
  popupContainer.style.padding = "10px";
  popupContainer.style.background = "white";
  popupContainer.style.border = "1px solid black";
  popupContainer.style.zIndex = "9999";

  // Create the popup text
  var popupText = document.createElement("span");
  popupText.innerText = "Translate this page to English?";
  popupText.style.marginRight = "10px";

  // Create the translate button
  var translateButton = document.createElement("button");
  translateButton.innerText = "Translate";
  translateButton.style.padding = "5px 10px";
  translateButton.style.background = "lightblue";
  translateButton.style.border = "none";
  translateButton.style.cursor = "pointer";

  // Add an event listener to the translate button
  translateButton.addEventListener("click", function() {
    // Get the current page URL
    var currentURL = window.location.href;

    // Redirect to the Google Translate page with the current URL
    var translateURL = "https://translate.google.com/translate?sl=auto&tl=en&u=" + encodeURIComponent(currentURL);
    window.location.href = translateURL;
  });

  // Append the popup text and translate button to the popup container
  popupContainer.appendChild(popupText);
  popupContainer.appendChild(translateButton);

  // Append the popup container to the document body
  document.body.appendChild(popupContainer);
}

 function googleTranslateElementInit() {
        alert("Start Google Translate");
         var popup = document.getElementById("myPopup");
         popup.classList.toggle("show");
                 alert("PopUp");
        new google.translate.TranslateElement({
            pageLanguage: 'kr', 
            includedLanguages: 'en', 
            autoDisplay: false
        }, 'google_translate_element');
        alert("Translate Element");
        var a = document.querySelector("#google_translate_element select");
        a.selectedIndex=1;
        a.dispatchEvent(new Event('change'));
     alert("End Google Translate");
    }

// Call the function to add the translate popup when the page loads
googleTranslateElementInit();
//addTranslatePopup();
//window.addEventListener("load", addTranslatePopup);
