let simulation = null;
let canvas = null;
let ctx = null;

let lastTime = 0;
let physicsCrashed = false;

const MAX_FRAME_DT = 1.0 / 20.0;
const MAX_SUBSTEP_DT = 1.0 / 240.0;

function isFiniteParticleValue(value) {
    return Number.isFinite(value);
}

function stepSimulationSafely(frameDt) {
    let remaining = Math.max(0, Math.min(frameDt, MAX_FRAME_DT));

    while (remaining > 0) {
        const stepDt = Math.min(remaining, MAX_SUBSTEP_DT);
        simulation.update(stepDt);
        remaining -= stepDt;
    }
}

function drawParticles() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    const particleCount = simulation.getParticleCount();

    for (let i = 0; i < particleCount; i++) {
        const x = simulation.getParticleX(i);
        const y = simulation.getParticleY(i);
        const radius = simulation.getParticleRadius(i);
        const charge = simulation.getParticleCharge(i);

        if (!isFiniteParticleValue(x) ||
            !isFiniteParticleValue(y) ||
            !isFiniteParticleValue(radius) ||
            radius <= 0) {
            console.warn("Skipping particle with invalid numeric state", {
                index: i,
                x,
                y,
                radius,
                charge
            });
            continue;
        }

        if (charge > 0) {
            ctx.fillStyle = "red";
        }
        else if (charge < 0) {
            ctx.fillStyle = "blue";
        }
        else {
            ctx.fillStyle = "white";
        }

        ctx.beginPath();
        ctx.arc(x, y, radius, 0, 2 * Math.PI);
        ctx.fill();

        if (simulation.hasSelected() && i === simulation.getSelectedIndex()) {
            ctx.strokeStyle = "yellow";
            ctx.lineWidth = 4;
            ctx.beginPath();
            ctx.arc(x, y, radius + 3, 0, 2 * Math.PI);
            ctx.stroke();
        }
    }
}

function animationLoop(currentTime) {
    const dt = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;

    if (simulation !== null && !physicsCrashed) {
        try {
            if (Number.isFinite(dt) && dt >= 0) {
                stepSimulationSafely(dt);
            }
            drawParticles();
        }
        catch (error) {
            physicsCrashed = true;
            console.error("Simulation update failed", error);
        }
    }

    requestAnimationFrame(animationLoop);
}

var Module = {
    onRuntimeInitialized: function () {
        console.log("ChargeLab WASM loaded!");

        canvas = document.getElementById("simCanvas");
        ctx = canvas.getContext("2d");

        simulation = new Module.Simulation(900, 700);

        simulation.addParticleAtFull(300, 350, 1.0, 10.0, 18.0);
        simulation.addParticleAtFull(600, 350, -1.0, 10.0, 18.0);

        simulation.setME0();

        console.log("Particle count:", simulation.getParticleCount());

        lastTime = performance.now();
        requestAnimationFrame(animationLoop);
    }
};