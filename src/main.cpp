#include <AudioFile.h>
#include <benchmark/benchmark.h>

#include <FDN.h>
#include <phonon.h>

#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

using namespace std::chrono_literals;

constexpr double kSampleRate = 48000.0;
constexpr int kFrameSize = 128;
constexpr double kDurationSeconds = 1.0;
constexpr size_t kBufferSize = static_cast<size_t>(kSampleRate * kDurationSeconds);

// constexpr std::array kFDNOrders = {4, 6, 8, 12, 16, 24, 32};

std::unique_ptr<FDN> CreateSilvinFDN(int order, MatrixType matType)
{
    std::unique_ptr<FDN> fdn = std::make_unique<FDN>();
    fdn->initialise(kSampleRate);

    fdn->changeFDNorder(order, matType);

    for (int i = 0; i < Global::numOctaveBands; ++i)
    {
        fdn->setRT(i, 1.0); // Set different reverb times for each delay line
    }

    fdn->changeDelayLineSetting(DelayLineSetting::primesPredef, 600, 10000);
    fdn->recalculateCoeffs(true);
    return fdn;
}

static void Silvin_FDN(benchmark::State& state)
{
    std::unique_ptr<FDN> bench_fdn = CreateSilvinFDN(state.range(0), MatrixType::randomMat);
    std::vector<double> inputSignal(kBufferSize, 0.0);
    inputSignal[0] = 1.0; // Impulse input

    std::vector<double> outputSignal(kBufferSize, 0.0);

    for (auto _ : state)
    {
        for (size_t n = 0; n < inputSignal.size(); ++n)
        {
            outputSignal[n] = bench_fdn->calculate(inputSignal[n]);
        }
        benchmark::DoNotOptimize(outputSignal);
    }
}
BENCHMARK(Silvin_FDN)->DenseRange(4, 32, 2)->Unit(benchmark::kMillisecond);

static void Phonon_Benchmark(benchmark::State& state)
{

    IPLContextSettings context_settings;
    context_settings.version = STEAMAUDIO_VERSION;
    context_settings.simdLevel = IPL_SIMDLEVEL_NEON;

    IPLContext context;
    IPLerror error = iplContextCreate(&context_settings, &context);
    if (error != IPL_STATUS_SUCCESS)
    {
        std::cout << "Error creating IPL context: " << error << std::endl;
        return;
    }

    IPLAudioBuffer in_buffer;
    iplAudioBufferAllocate(context, 1, kFrameSize, &in_buffer);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    for (size_t i = 0; i < kFrameSize; ++i)
    {
        in_buffer.data[0][i] = static_cast<float>(dis(gen));
    }
    IPLAudioBuffer out_buffer;
    iplAudioBufferAllocate(context, 4, kFrameSize, &out_buffer);

    IPLAudioSettings audio_settings;
    audio_settings.samplingRate = static_cast<IPLint32>(kSampleRate);
    audio_settings.frameSize = kFrameSize;

    IPLReflectionEffectParams refl_params;
    refl_params.type = IPL_REFLECTIONEFFECTTYPE_PARAMETRIC;
    refl_params.ir = nullptr;
    refl_params.reverbTimes[0] = 1.0f;
    refl_params.reverbTimes[1] = 1.0f;
    refl_params.reverbTimes[2] = 1.0f;
    refl_params.numChannels = 1;
    // refl_params.irSize = 1;

    IPLReflectionEffectSettings refl_settings;
    refl_settings.type = IPL_REFLECTIONEFFECTTYPE_PARAMETRIC;
    refl_settings.irSize = 1;
    refl_settings.numChannels = 1;

    IPLReflectionEffect effect;
    error = iplReflectionEffectCreate(context, &audio_settings, &refl_settings, &effect);
    if (error != IPL_STATUS_SUCCESS)
    {
        std::cout << "Error creating reflection effect: " << error << std::endl;
        iplContextRelease(&context);
        return;
    }

    for (auto _ : state)
    {
        const auto frameCount = static_cast<size_t>(kBufferSize / kFrameSize);
        for (size_t frame = 0; frame < frameCount; ++frame)
        {
            iplReflectionEffectApply(effect, &refl_params, &in_buffer, &out_buffer, nullptr);
        }
    }
    benchmark::DoNotOptimize(in_buffer);
    benchmark::DoNotOptimize(out_buffer);

    iplAudioBufferFree(context, &in_buffer);
    iplAudioBufferFree(context, &out_buffer);

    iplReflectionEffectRelease(&effect);
    iplContextRelease(&context);
}
BENCHMARK(Phonon_Benchmark)->Unit(benchmark::kMillisecond);

// Run the benchmark
BENCHMARK_MAIN();

// TEST_CASE("Silvin_FDN")
// {
//     constexpr int kFDNOrder = 8;
//     std::unique_ptr<FDN> fdn = CreateSilvinFDN(kFDNOrder, MatrixType::hadamard);

//     std::vector<double> inputSignal(kBufferSize, 0.0);
//     inputSignal[0] = 1.0; // Impulse input

//     std::vector<double> outputSignal(kBufferSize, 0.0);
//     for (size_t n = 0; n < inputSignal.size(); ++n)
//     {
//         outputSignal[n] = fdn->calculate(inputSignal[n]);
//     }

//     AudioFile<double> audioFile;
//     audioFile.setSampleRate(static_cast<int>(kSampleRate));
//     audioFile.setAudioBufferSize(1, static_cast<int>(outputSignal.size()));
//     audioFile.setAudioBuffer({outputSignal});
//     audioFile.setBitDepth(24);
//     audioFile.save("silvin_fdn_output.wav");

//     for (auto order : kFDNOrders)
//     {
//         std::unique_ptr<FDN> bench_fdn = CreateSilvinFDN(order, MatrixType::randomMat);

//         std::string title = "Silvin FDN Order " + std::to_string(order);

//         for (size_t n = 0; n < inputSignal.size(); ++n)
//         {
//             outputSignal[n] = bench_fdn->calculate(inputSignal[n]);
//             // outputSignal[n] = 2 * std::sin(n);
//         }
//     }
// }