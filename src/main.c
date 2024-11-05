#include <efi/efi.h>
#include <efi/efilib.h>
#include <stdbool.h>
#include <stdarg.h>

static inline void PlotPixel_32bpp(uint32_t* framebuffer, uint32_t pixels_per_scan_line, int x, int y, uint32_t pixel) {
   *((uint32_t*)(framebuffer + pixels_per_scan_line * y + x)) = pixel;
}

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  // Initialize EFI::lib
  InitializeLib(ImageHandle, SystemTable);
  Print(L"EFI Loader :: Starting Session");

  // Locating GOP protocol data (for VESA usage)
  EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
  EFI_STATUS Status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
  if(EFI_ERROR(Status)) {
    Print(L"EFI Loader :: Unable to locate GOP\n\r");
  } else {
    Print(L"EFI Loader :: GOP has been located\n\r");
  }

  // Query GOP modes
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
  UINTN SizeOfInfo, numModes, nativeMode;
  Status = uefi_call_wrapper(gop->QueryMode, 4, gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
  // this is needed to get the current video mode
  if (Status == EFI_NOT_STARTED)
    Status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);
  if(EFI_ERROR(Status)) {
    Print(L"EFI Loader :: Unable to get native mode\n\r");
  } else {
    nativeMode = gop->Mode->Mode;
    numModes = gop->Mode->MaxMode;
    Print(L"EFI Loader :: Native mode is %d\n\r", nativeMode);
    Print(L"EFI Loader :: Number of modes is %d\n\r", numModes);
  }

  // Display modes
  for (uint32_t i = 0; i < numModes; i++) {
    Status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo, &info);
    Print(L"EFI Loader :: Mode %d has width %d and height %d, format is %x%s\n\r",
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
    Print(L"EFI Loader :: Unable to set mode %03d\n\r", graphic_mode);
  } else {
    // get framebuffer
    Print(L"EFI Loader :: Framebuffer address %x size %d, width %d height %d pixelsperline %d\n\r",
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
  (void)uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);
  Print(L"EFI Loader :: Ending Session\n\r");
  while(true);
  return EFI_SUCCESS;
}
