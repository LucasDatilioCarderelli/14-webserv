window.onload = function () {
    var container = document.querySelector('.container');
    var opacity = 0;
    var intervalId = setInterval(function () {
        if (opacity < 1) {
            opacity = opacity + 0.01;
            container.style.opacity = opacity;
        } else {
            clearInterval(intervalId);
        }
    }, 10);
};