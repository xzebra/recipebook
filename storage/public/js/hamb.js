let hamburger = document.getElementById("hamburger");
hamburger.onclick = function() {
    let menu = document.getElementById("menu-bar");
    if(menu.style.display !== "block") {
        menu.style.display = "block";
        hamburger.classList.add("open");
    } else {
        menu.style.display = "none";
        hamburger.classList.remove("open");
    }
}