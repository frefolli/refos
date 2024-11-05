#include <climits>
#include <cstdarg>

extern "C" {
  #include <efi/efi.h>
  #include <efi/efilib.h>
}

#include <optional>

inline void PlotPixel_32bpp(uint32_t* framebuffer, uint32_t pixels_per_scan_line, int x, int y, uint32_t pixel) {
   *((uint32_t*)(framebuffer + pixels_per_scan_line * y + x)) = pixel;
}

namespace os {
  namespace video {
    struct Framebuffer {
        uint32_t* address;
        uint32_t size;
        uint32_t width;
        uint32_t height;
        uint32_t pitch;
    };
    struct Preference {
      uint32_t width;
      uint32_t height;
    };
  }
}

template <typename T>
inline T abs(T t) {
  if (t >= 0)
    return t;
  return -t;
}

namespace efi {
  static EFI_HANDLE IMAGE_HANDLE = nullptr;
  static EFI_SYSTEM_TABLE *SYSTEM_TABLE = nullptr;

  void init(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    IMAGE_HANDLE = ImageHandle;
    SYSTEM_TABLE = SystemTable;
  }
  
  void printf(const wchar_t* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    VPrint(fmt, args);
    va_end(args);
  }
  
  void clear() {
    uefi_call_wrapper((void*)SYSTEM_TABLE->ConOut->ClearScreen, 1, SYSTEM_TABLE->ConOut);
  }

  std::optional<os::video::Framebuffer> get_video_framebuffer(const os::video::Preference& preference) {
    // Locating GOP protocol data (for VESA usage)
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_STATUS Status = uefi_call_wrapper((void*)BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
    if(EFI_ERROR(Status)) {
      efi::printf(L"EFI Loader :: Unable to locate GOP\n\r");
      return std::nullopt;
    }

    // Query GOP modes
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN SizeOfInfo, numModes, nativeMode;
    Status = uefi_call_wrapper((void*)gop->QueryMode, 4, gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
    // this is needed to get the current video mode
    if (Status == EFI_NOT_STARTED)
      Status = uefi_call_wrapper((void*)gop->SetMode, 2, gop, 0);
    if(EFI_ERROR(Status)) {
      efi::printf(L"EFI Loader :: Unable to get native mode\n\r");
      return std::nullopt;
    }

    nativeMode = gop->Mode->Mode;
    numModes = gop->Mode->MaxMode;
    efi::printf(L"EFI Loader :: Native mode is %d\n\r", nativeMode);
    efi::printf(L"EFI Loader :: Number of modes is %d\n\r", numModes);

    // Display modes
    UINTN graphic_mode = 0;
    UINTN mode_score = -1;
    for (uint32_t i = 0; i < numModes; i++) {
      Status = uefi_call_wrapper((void*)gop->QueryMode, 4, gop, i, &SizeOfInfo, &info);
      if(EFI_ERROR(Status)) {
        efi::printf(L"EFI Loader :: Unable to query mode %03d\n\r", i);
        return std::nullopt;
      }
      if (i == nativeMode) {
        efi::printf(L"EFI Loader :: Native Mode is id=%d and has width %d and height %d, pixel format is %x%s\n\r",
            i,
            info->HorizontalResolution,
            info->VerticalResolution,
            info->PixelFormat,
            i == nativeMode ? "(current)" : ""
            ); 
      }
      int32_t score = abs((int32_t)preference.width - (int32_t)info->HorizontalResolution) + abs((int32_t)preference.height - (int32_t)info->VerticalResolution);
      if (score < mode_score) {
        mode_score = score;
        graphic_mode = i;
      }
    }

    // Select mode
    Status = uefi_call_wrapper((void*)gop->SetMode, 2, gop, graphic_mode);
    if(EFI_ERROR(Status)) {
      efi::printf(L"EFI Loader :: Unable to set mode %03d\n\r", graphic_mode);
      return std::nullopt;
    }

    os::video::Framebuffer framebuffer;
    framebuffer.address = (uint32_t*)gop->Mode->FrameBufferBase;
    framebuffer.size =  gop->Mode->FrameBufferSize;
    framebuffer.width =  gop->Mode->Info->HorizontalResolution;
    framebuffer.height =  gop->Mode->Info->VerticalResolution;
    framebuffer.pitch =  gop->Mode->Info->PixelsPerScanLine;
    return framebuffer;
  }
}

extern "C" EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  // Initialize EFI::lib
  efi::init(ImageHandle, SystemTable);
  efi::clear();
  efi::printf(L"EFI Loader :: Starting Session\n\r");

#if 1
  auto maybe_framebuffer = efi::get_video_framebuffer({.width = 1024, .height = 768});
  if (maybe_framebuffer.has_value()) {
    os::video::Framebuffer framebuffer = maybe_framebuffer.value();
    efi::printf(L"EFI Loader :: Acquired Framebuffer %p with %dx%d screen\n\r",
        framebuffer.address,
        framebuffer.width,
        framebuffer.height);
    for (uint32_t x = 0; x < framebuffer.width; ++x) {
      for (uint32_t y = 0; y < framebuffer.height; ++y) {
        PlotPixel_32bpp(framebuffer.address, framebuffer.pitch, x, y, 0xFFFFFFFF);
      }
    }
  }
#endif

  // Hanging
  efi::printf(L"EFI Loader :: Ending Session\n\r");
  while(true);
  return EFI_SUCCESS;
}
