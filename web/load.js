const canvas = document.createElement('canvas')

// Pass functions to C++ 
const env = {
  atanf: x => Math.atan(x),
  Resize: (w, h) => {
    canvas.width = w
    canvas.height = h
    canvas.style.height = `min(100vh, ${100 * h / w}vw)`
  }
}

const { instance } = await WebAssembly.instantiateStreaming(
  fetch('./raycaster.wasm'),
  { env }
)

console.log(instance.exports)

// Create an image buffer from the WASM allocated memory
const image_buffer = new Uint8ClampedArray(
  instance.exports.memory.buffer,
  instance.exports.InitAndGetPointer(),
  canvas.width * canvas.height * 4
)
const image_data = new ImageData(image_buffer, canvas.width)

let prev_time = performance.now();
const frame = () => {
  const time = performance.now();
  document.title = 'Raycast engine - FPS: ' + ~~(1000 / (time - prev_time))
  instance.exports.GameLoop(time - prev_time)
  canvas.getContext('2d').putImageData(image_data, 0, 0)
  prev_time = time;
  setTimeout(frame, 0);
}

addEventListener('keydown', e => {
  canvas.requestPointerLock();
  switch (e.key) {
    case 'w': instance.exports.KeyDown(0); break;
    case 's': instance.exports.KeyDown(1); break;
    case 'a': instance.exports.KeyDown(2); break;
    case 'd': instance.exports.KeyDown(3); break;
    default: instance.exports.KeyDown(4); break;
  }
})

addEventListener('keyup', e => {
  switch (e.key) {
    case 'w': instance.exports.KeyUp(0); break;
    case 's': instance.exports.KeyUp(1); break;
    case 'a': instance.exports.KeyUp(2); break;
    case 'd': instance.exports.KeyUp(3); break;
    default: instance.exports.KeyUp(4); break;
  }
})

function handle_mouse(e) {
  // Normalize pointer movement so the sensitivity stays the same even on
  // smaller canvas
  const scaled_height = parseFloat(getComputedStyle(canvas).height);
  const scaled_width = parseFloat(getComputedStyle(canvas).width);
  const normalized_x = e.movementX * canvas.width / scaled_width;
  const normalized_y = e.movementY * canvas.height / scaled_height;
  instance.exports.MouseMove(normalized_x, normalized_y);
}

addEventListener('pointerlockchange', () => {
  if (document.pointerLockElement == canvas) {
    document.addEventListener('mousemove', handle_mouse)
  } else {
    document.removeEventListener('mousemove', handle_mouse)
  }
});

// Start the game
document.body.appendChild(canvas)
frame();
