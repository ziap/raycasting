const canvas = document.createElement("canvas");

// Pass functions to C++
const env = {
  Resize: (w, h) => {
    canvas.width = w;
    canvas.height = h;
    canvas.style.height = `min(100vh, ${(100 * h) / w}vw)`;
  },
};

const { instance } = await WebAssembly.instantiateStreaming(
  fetch("./raycaster.wasm"),
  { env }
);

// Create an image buffer from the WASM allocated memory
const image_data = new ImageData(
  new Uint8ClampedArray(
    instance.exports.memory.buffer,
    instance.exports.InitAndGetPointer(),
    canvas.width * canvas.height * 4
  ),
  canvas.width
);

let prev_time;
function first(time) {
  prev_time = time;
  requestAnimationFrame(frame);
}
function frame(time) {
  instance.exports.GameLoop(time - prev_time);
  canvas.getContext("2d").putImageData(image_data, 0, 0);
  prev_time = time;
  requestAnimationFrame(frame);
}

addEventListener("keydown", (e) => {
  switch (e.key) {
    case "w":
      instance.exports.KeyDown(0);
      break;
    case "s":
      instance.exports.KeyDown(1);
      break;
    case "a":
      instance.exports.KeyDown(2);
      break;
    case "d":
      instance.exports.KeyDown(3);
      break;
    default:
      instance.exports.KeyDown(4);
      break;
  }
});

addEventListener("keyup", (e) => {
  switch (e.key) {
    case "w":
      instance.exports.KeyUp(0);
      break;
    case "s":
      instance.exports.KeyUp(1);
      break;
    case "a":
      instance.exports.KeyUp(2);
      break;
    case "d":
      instance.exports.KeyUp(3);
      break;
    default:
      instance.exports.KeyUp(4);
      break;
  }
});

let is_dragging = false
document.addEventListener("mousedown", () => is_dragging = true)
document.addEventListener("mouseup", () => is_dragging = false)

document.addEventListener("mousemove", (e) => {
  if (!is_dragging) return;
  const scaled_height = parseFloat(getComputedStyle(canvas).height);
  const scaled_width = parseFloat(getComputedStyle(canvas).width);
  const normalized_x = (e.movementX * canvas.width) / scaled_width;
  const normalized_y = (e.movementY * canvas.height) / scaled_height;
  instance.exports.MouseMove(normalized_x, normalized_y);
});

// TODO: Implement touch control:
// - Left side of screen: Move player
// - Right side of screen: Move camera

// Start the game
document.body.appendChild(canvas);
requestAnimationFrame(first);
