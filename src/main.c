#include <efi/efi.h>
#include <efi/efilib.h>
#include <stdbool.h>

#define loader_println(fmt_or_msg, ...) \
  Print(L"EFI Loader :: "); \
  Print(fmt_or_msg __VA_OPT__(,)  __VA_ARGS__); \
  Print(L"\n\r")

static inline void PlotPixel_32bpp(uint32_t* framebuffer, uint32_t pixels_per_scan_line, int x, int y, uint32_t pixel) {
   *((uint32_t*)(framebuffer + pixels_per_scan_line * y + x)) = pixel;
}

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  // Initialize EFI::lib
  InitializeLib(ImageHandle, SystemTable);
  loader_println(L"Starting Session");

  // Locating GOP protocol data (for VESA usage)
  EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
  EFI_STATUS Status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
  if(EFI_ERROR(Status)) {
    loader_println(L"Unable to locate GOP");
  } else {
    loader_println(L"GOP has been located");
  }

  // Query GOP modes
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
  UINTN SizeOfInfo, numModes, nativeMode;
  Status = uefi_call_wrapper(gop->QueryMode, 4, gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
  // this is needed to get the current video mode
  if (Status == EFI_NOT_STARTED)
    Status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);
  if(EFI_ERROR(Status)) {
    loader_println(L"Unable to get native mode");
  } else {
    nativeMode = gop->Mode->Mode;
    numModes = gop->Mode->MaxMode;
    loader_println(L"Native mode is %d", nativeMode);
    loader_println(L"Number of modes is %d", numModes);
  }

  // Display modes
  for (uint32_t i = 0; i < numModes; i++) {
    Status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo, &info);
    loader_println(L"Mode %d has width %d and height %d, format is %x%s",
        i,
        info->HorizontalResolution,
        info->VerticalResolution,
        info->PixelFormat,
        i == nativeMode ? "(current)" : ""
        );
  }

  // Select mode
  UINTN graphic_mode = 15; // 1366x768
  Status = uefi_call_wrapper(gop->SetMode, 2, gop, graphic_mode);
  if(EFI_ERROR(Status)) {
    loader_println(L"Unable to set mode %03d", graphic_mode);
  } else {
    // get framebuffer
    loader_println(L"Framebuffer address %x size %d, width %d height %d pixelsperline %d",
        gop->Mode->FrameBufferBase,
        gop->Mode->FrameBufferSize,
        gop->Mode->Info->HorizontalResolution,
        gop->Mode->Info->VerticalResolution,
        gop->Mode->Info->PixelsPerScanLine
        );
  }

  for (uint32_t x = 0; x < gop->Mode->Info->HorizontalResolution; ++x) {
    for (uint32_t y = 0; y < gop->Mode->Info->VerticalResolution; ++y) {
      PlotPixel_32bpp((uint32_t*)gop->Mode->FrameBufferBase, gop->Mode->Info->PixelsPerScanLine, x, y, 0xFFFFFFFF);
    }
  }

  // Hanging
  loader_println(L"Ending Session");
  while(true);
  return EFI_SUCCESS;
}
